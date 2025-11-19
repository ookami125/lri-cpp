import cv2
import numpy as np
import sys
from PIL import Image
from pathlib import Path

def blur_edges(image, border_width):
    height, width, channels = image.shape
    top_left = (border_width, border_width)
    bottom_right = (width - border_width, height - border_width)
    mask = np.zeros((height, width, 1), dtype=np.uint8)
    cv2.rectangle(mask, top_left, bottom_right, (255), thickness=cv2.FILLED)
    if image.shape[2] == 3:
        image = cv2.cvtColor(image, cv2.COLOR_BGR2BGRA)
    image[:,:,3] = mask[:,:,0]
    if border_width % 2 == 0:
        border_width += 1
    image[:,:,3] = cv2.GaussianBlur(image[:,:,3], (border_width * 2 - 1, border_width * 2 - 1), 0)
    return image

def generate_gaussian_pyramid(image, levels):
    gaussian_pyramid = [image]

    for _ in range(levels):
        image = cv2.pyrDown(image)
        gaussian_pyramid.append(image)

    return gaussian_pyramid

def generate_laplacian_pyramid(gaussian_pyramid):
    laplacian_pyramid = []

    for i in range(len(gaussian_pyramid) - 1):
        dstsize = (gaussian_pyramid[i].shape[1], gaussian_pyramid[i].shape[0])

        upsampled = cv2.pyrUp(gaussian_pyramid[i+1], dstsize=dstsize)
        laplacian = cv2.subtract(gaussian_pyramid[i], upsampled)

        laplacian_pyramid.append(laplacian)
    
    laplacian_pyramid.append(gaussian_pyramid[-1])

    return laplacian_pyramid

def blend_pyramids(laplacianA, laplacianB, mask_pyramid):
    blended_pyramid = []

    for lapA, lapB, mask in zip(laplacianA, laplacianB, mask_pyramid):
        blended = lapA * mask + lapB * (1 - mask)
        blended_pyramid.append(blended)

    return blended_pyramid

def reconstruct_from_pyramid(laplacian_pyramid):
    image = laplacian_pyramid[-1]

    for i in range(len(laplacian_pyramid) - 2, -1, -1):
        dstsize = (laplacian_pyramid[i].shape[1], laplacian_pyramid[i].shape[0])
        image = cv2.pyrUp(image, dstsize=dstsize)
        image = cv2.add(image, laplacian_pyramid[i])
    
    return image

def debug_write(name, pyramid):
    i = 0
    for image in pyramid:
        cv2.imwrite(f'./debug/{name}_{i}.png', image)
        i += 1

def get_laplacian_pyramid(image, levels):
    gaussianA = generate_gaussian_pyramid(image.astype(np.float32), levels)
    laplacianA = generate_laplacian_pyramid(gaussianA)
    return laplacianA

def laplacian_pyramid_blending(img1, img2, mask, levels=6):
    mask = mask.astype(np.float32) / 255.0

    laplacianA = get_laplacian_pyramid(img1.astype(np.float32), levels)
    laplacianB = get_laplacian_pyramid(img2.astype(np.float32), levels)
    
    gaussianMask = generate_gaussian_pyramid(mask, levels)
    
    blended_pyramids = blend_pyramids(laplacianA, laplacianB, gaussianMask)

    return blended_pyramids

def merge_images(image1, image2):
    b, g, r, alpha = cv2.split(image2)
    #alpha_mask = alpha.astype(float)# / 255.0
    alpha_mask = 255 - np.stack([alpha] * 3, axis=-1)
    
    image2 = np.stack([b,g,r], axis=-1)

    cv2.imwrite("./debug/image1.png", image1)
    cv2.imwrite("./debug/image2.png", image2)
    cv2.imwrite("./debug/mask.png", alpha_mask)

    composite = laplacian_pyramid_blending(image1, image2, alpha_mask, levels=6)
    #cv2.imwrite("./debug/composite.png", composite)

    return composite

def white_balance_match(settings, dest_img):
    balanced_img = np.zeros_like(dest_img) #Initialize final image

    print(settings)

    for i in range(3): #i stands for the channel index 
        balanced_img[...,i] = np.clip(dest_img[...,i], settings["bmin"][i], settings["bmax"][i])
        balanced_img[...,i] = (balanced_img[...,i]-settings["bmin"][i]) / (settings["bmax"][i] - settings["bmin"][i]) * 255
    return balanced_img

def ensure_alpha_channel(image):
    # Check if the image has an alpha channel (4 channels)
    if image.shape[2] == 3:  # If it has 3 channels (BGR)
        # Add an alpha channel filled with 255 (opaque)
        alpha_channel = np.ones((image.shape[0], image.shape[1]), dtype=np.uint8) * 255
        image = cv2.merge((image, alpha_channel))  # Merge the original image with the alpha channel
    return image

def calculate_cdf(histogram):
    """
    This method calculates the cumulative distribution function
    :param array histogram: The values of the histogram
    :return: normalized_cdf: The normalized cumulative distribution function
    :rtype: array
    """
    # Get the cumulative sum of the elements
    cdf = histogram.cumsum()
 
    # Normalize the cdf
    normalized_cdf = cdf / float(cdf.max())
 
    return normalized_cdf
 
def calculate_lookup(src_cdf, ref_cdf):
    """
    This method creates the lookup table
    :param array src_cdf: The cdf for the source image
    :param array ref_cdf: The cdf for the reference image
    :return: lookup_table: The lookup table
    :rtype: array
    """
    lookup_table = np.zeros(256)
    lookup_val = 0
    for src_pixel_val in range(len(src_cdf)):
        lookup_val
        for ref_pixel_val in range(len(ref_cdf)):
            if ref_cdf[ref_pixel_val] >= src_cdf[src_pixel_val]:
                lookup_val = ref_pixel_val
                break
        lookup_table[src_pixel_val] = lookup_val
    return lookup_table
 
def match_histograms(src_image, ref_image):
    """
    This method matches the source image histogram to the
    reference signal
    :param image src_image: The original source image
    :param image  ref_image: The reference image
    :return: image_after_matching
    :rtype: image (array)
    """
    # Split the images into the different color channels
    # b means blue, g means green and r means red
    src = cv2.split(src_image)
    src_b, src_g, src_r = src[0], src[1], src[2]
    ref_b, ref_g, ref_r = cv2.split(ref_image)
 
    # Compute the b, g, and r histograms separately
    # The flatten() Numpy method returns a copy of the array c
    # collapsed into one dimension.
    src_hist_blue, bin_0 = np.histogram(src_b.flatten(), 256, [0,256])
    src_hist_green, bin_1 = np.histogram(src_g.flatten(), 256, [0,256])
    src_hist_red, bin_2 = np.histogram(src_r.flatten(), 256, [0,256])    
    ref_hist_blue, bin_3 = np.histogram(ref_b.flatten(), 256, [0,256])    
    ref_hist_green, bin_4 = np.histogram(ref_g.flatten(), 256, [0,256])
    ref_hist_red, bin_5 = np.histogram(ref_r.flatten(), 256, [0,256])
 
    # Compute the normalized cdf for the source and reference image
    src_cdf_blue = calculate_cdf(src_hist_blue)
    src_cdf_green = calculate_cdf(src_hist_green)
    src_cdf_red = calculate_cdf(src_hist_red)
    ref_cdf_blue = calculate_cdf(ref_hist_blue)
    ref_cdf_green = calculate_cdf(ref_hist_green)
    ref_cdf_red = calculate_cdf(ref_hist_red)
 
    # Make a separate lookup table for each color
    blue_lookup_table = calculate_lookup(src_cdf_blue, ref_cdf_blue)
    green_lookup_table = calculate_lookup(src_cdf_green, ref_cdf_green)
    red_lookup_table = calculate_lookup(src_cdf_red, ref_cdf_red)
 
    # Use the lookup function to transform the colors of the original
    # source image
    blue_after_transform = cv2.LUT(src_b, blue_lookup_table)
    green_after_transform = cv2.LUT(src_g, green_lookup_table)
    red_after_transform = cv2.LUT(src_r, red_lookup_table)
 
    # Put the image back together
    #print(blue_after_transform.depth(), green_after_transform.depth(), red_after_transform.depth(), src_a)
    image_after_matching = cv2.merge([blue_after_transform, green_after_transform, red_after_transform])

    image_after_matching = cv2.convertScaleAbs(image_after_matching)

    # Ensure the target image has an alpha channel (BGRA)
    if src_image.shape[2] == 3:
        src_image = cv2.cvtColor(src_image, cv2.COLOR_BGR2BGRA)

    # Extract the alpha channel from the original target image
    alpha_channel = src_image[:, :, 3]

    # Create a new BGRA image by merging the modified BGR with the original alpha channel
    result_bgra = cv2.merge((image_after_matching[:, :, 0], image_after_matching[:, :, 1], image_after_matching[:, :, 2], alpha_channel))
 
    return result_bgra

def copy_l_channel(source_img, target_img):
    # Convert source and target images to LUV color space
    source_luv = cv2.cvtColor(source_img, cv2.COLOR_BGR2Luv)
    target_luv = cv2.cvtColor(target_img, cv2.COLOR_BGR2Luv)

    # Split the LUV channels
    L_source, U_source, V_source = cv2.split(source_luv)
    L_target, U_target, V_target = cv2.split(target_luv)

    # Copy the L channel from source to target
    L_target[:] = (L_target/2 + L_source/2)
    U_target[:] = (U_target/2 + U_source/2)
    V_target[:] = (V_target/2 + V_source/2)

    # Merge the modified L channel back with U and V channels of the target
    modified_luv = cv2.merge((L_target, U_target, V_target))

    # Convert back to BGR
    modified_bgr = cv2.cvtColor(modified_luv, cv2.COLOR_Luv2BGR)

    # Ensure the target image has an alpha channel (BGRA)
    if target_img.shape[2] == 3:
        target_img = cv2.cvtColor(target_img, cv2.COLOR_BGR2BGRA)

    # Extract the alpha channel from the original target image
    alpha_channel = target_img[:, :, 3]

    # Create a new BGRA image by merging the modified BGR with the original alpha channel
    result_bgra = cv2.merge((modified_bgr[:, :, 0], modified_bgr[:, :, 1], modified_bgr[:, :, 2], alpha_channel))

    return result_bgra

def generate_noise_image(width, height, noise_type='gaussian', mean=0, stddev=25):
    """
    Generates a noise image.

    Parameters:
    - width: Width of the noise image.
    - height: Height of the noise image.
    - noise_type: Type of noise ('gaussian' or 'uniform').
    - mean: Mean for Gaussian noise.
    - stddev: Standard deviation for Gaussian noise.

    Returns:
    - Noise image (numpy array).
    """

    if noise_type == 'gaussian':
        # Generate Gaussian noise
        noise = np.random.normal(mean, stddev, (height, width, 3)).astype(np.uint8)
    
    elif noise_type == 'uniform':
        # Generate uniform noise
        noise = np.random.uniform(0, 255, (height, width, 3)).astype(np.uint8)

    # Ensure values are in the correct range for uint8
    noise = np.clip(noise, 0, 255).astype(np.uint8)

    return noise

def match_histograms(source, reference):
    """
    Match the histogram of the source image to that of the reference image.
    
    Args:
        source (ndarray): The source image.
        reference (ndarray): The reference image to match the exposure to.

    Returns:
        matched (ndarray): The exposure-matched source image.
    """
    # Convert images to grayscale
    source_gray = cv2.cvtColor(source, cv2.COLOR_BGR2GRAY)
    reference_gray = cv2.cvtColor(reference, cv2.COLOR_BGR2GRAY)
    
    # Calculate the cumulative histogram for both images
    source_hist, bins = np.histogram(source_gray.flatten(), 256, [0, 256])
    reference_hist, _ = np.histogram(reference_gray.flatten(), 256, [0, 256])

    # Compute cumulative distribution functions (CDF)
    source_cdf = source_hist.cumsum()
    reference_cdf = reference_hist.cumsum()

    # Normalize CDFs to range 0-255
    source_cdf_normalized = source_cdf * float(255) / source_cdf[-1]
    reference_cdf_normalized = reference_cdf * float(255) / reference_cdf[-1]

    # Create a lookup table to map source pixel values to reference pixel values
    lut = np.interp(source_cdf_normalized, reference_cdf_normalized, bins[:-1])

    # Apply the LUT to the source image to adjust exposure
    matched = cv2.LUT(source, np.uint8(lut))

    return matched

def match_exposure_across_images(images):
    """
    Match the exposure of multiple images to the exposure of the first image in the list.
    
    Args:
        images (list): List of images (ndarray) to adjust.
        
    Returns:
        matched_images (list): List of exposure-matched images.
    """
    # Take the first image as the reference for exposure matching
    reference_image = images[0]
    
    matched_images = []
    
    for img in images:
        matched_image = match_histograms(img, reference_image)
        matched_images.append(matched_image)
    
    return matched_images

def process_images(image_files):
    #images = [cv2.imread(image_file) for image_file in image_files]

    blend_levels = 6

    images = match_exposure_across_images([cv2.imread(image_file) for image_file in image_files])
    
    base_image = images[0]
    height, width, _ = base_image.shape
    width *= 2
    height *= 2
    base_image = cv2.resize(base_image, (width, height), interpolation=cv2.INTER_CUBIC)
    #base_image = white_balance(base_image)

    cv2.imwrite("./debug/base_image.png", base_image)

    white_balence_settings = {
        "bmin": [[], [], []],
        "bmax": [[], [], []],
    }

    # if True:
    #     stitched_image = np.array(cv2.imread(images[0]))

    #     for image in images[1:]:
    #         dest_img = cv2.imread(image)
    #         stitched_image = np.concatenate((stitched_image, dest_img), axis=0)

    #     for i in range(3):
    #         hist, bins = np.histogram(stitched_image[..., i].ravel(), 256, (0, 256))
    #         white_balence_settings["bmin"][i] = np.min(np.where(hist>(hist.sum()*0.0005)))
    #         white_balence_settings["bmax"][i] = np.max(np.where(hist>(hist.sum()*0.0005)))


    #base_image = white_balance_match(white_balence_settings, base_image)
    base_pyramid = get_laplacian_pyramid(base_image.astype(np.float32), blend_levels)

    count = 0
    for image in images[1:]:
        print(f"{count}")
        count += 1
        overlay_image = image# cv2.imread(image_file)  # Load the next image
        #overlay_image = white_balance_match(white_balence_settings, overlay_image)

        base_image = reconstruct_from_pyramid(base_pyramid)

        gray_base = cv2.cvtColor(base_image, cv2.COLOR_BGR2GRAY)
        gray_overlay = cv2.cvtColor(overlay_image, cv2.COLOR_BGR2GRAY)

        sift = cv2.SIFT_create()
        keypoints_base, descriptors_base = sift.detectAndCompute(gray_base.astype(np.uint8), None)
        keypoints_overlay, descriptors_overlay = sift.detectAndCompute(gray_overlay, None)

        index_params = dict(algorithm=1, trees=5)
        search_params = dict(checks=50)
        flann = cv2.FlannBasedMatcher(index_params, search_params)

        matches = flann.knnMatch(descriptors_base, descriptors_overlay, k=2)

        good_matches = []
        for m, n in matches:
            if m.distance < 0.7 * n.distance:
                good_matches.append(m)

        if len(good_matches) >= 4:
            points_base = np.zeros((len(good_matches), 2), dtype=np.float32)
            points_overlay = np.zeros((len(good_matches), 2), dtype=np.float32)

            for i, match in enumerate(good_matches):
                points_base[i, :] = keypoints_base[match.queryIdx].pt
                points_overlay[i, :] = keypoints_overlay[match.trainIdx].pt


            h, mask = cv2.findHomography(points_overlay, points_base, cv2.RANSAC)
            
            unwarped_base = cv2.warpPerspective(base_image, h, (width//2, height//2), flags=cv2.WARP_INVERSE_MAP)

            whited_overlay = blur_edges(overlay_image, 100)
            cv2.imwrite("./debug/whited_overlay.png", whited_overlay)

            warped_overlay = cv2.warpPerspective(whited_overlay, h, (width, height))

            cv2.imwrite("./debug/warped_overlay.png", warped_overlay)

            b, g, r, alpha = cv2.split(warped_overlay)
            alpha_mask = np.stack([alpha] * 3, axis=-1)
            cv2.imwrite("./debug/alpha_mask.png", alpha_mask)

            warped_overlay = np.stack([b,g,r], axis=-1)

            warped_pyramid = get_laplacian_pyramid(warped_overlay.astype(np.float32), blend_levels)

            gaussianMask = generate_gaussian_pyramid(alpha_mask.astype(np.float32) / 255.0, blend_levels)

            base_pyramid = blend_pyramids(warped_pyramid, base_pyramid, gaussianMask)

        else:
            print(f"Not enough matches found to compute homography for {image_file}.")
        
        #break

    base_image = reconstruct_from_pyramid(base_pyramid).astype(np.uint8)
    return base_image

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <image1> <image2> ...")
        sys.exit(1)

    images = sys.argv[1:]  # Get the list of image files from argv
    composite_image = process_images(images)
    cv2.imwrite('composite.png', composite_image)  # Save the final composite image
