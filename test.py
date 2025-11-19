import cv2
import numpy as np
import sys

src_img = cv2.imread("buffers/00-PACKED_10BPP.png")

images = sys.argv[1:]

stitched_image = np.array(cv2.imread(images[0]))

for image in images[1:]:
    dest_img = cv2.imread(image)
    stitched_image = np.concatenate((stitched_image, dest_img), axis=0)

cv2.imwrite("./debug/stitched_image.png", stitched_image)

j = 0
for image in images:
    dest_img = cv2.imread(image)
    balanced_img = np.zeros_like(dest_img)
    for i in range(3):
        hist, bins = np.histogram(stitched_image[..., i].ravel(), 256, (0, 256))
        bmin = np.min(np.where(hist>(hist.sum()*0.0005)))
        bmax = np.max(np.where(hist>(hist.sum()*0.0005)))
        balanced_img[...,i] = np.clip(dest_img[...,i], bmin, bmax)
        balanced_img[...,i] = (balanced_img[...,i]-bmin) / (bmax - bmin) * 255
    cv2.imwrite(f"./debug/balanced_img_{j}.png", balanced_img)
    j += 1

# print()