# Yaşar Polatlı
# 250201075

import numpy as np
from cv2 import cv2
from matplotlib import pyplot as plt

img00 = cv2.imread("data/goldengate-00.png", cv2.IMREAD_GRAYSCALE)
img01 = cv2.imread("data/goldengate-01.png", cv2.IMREAD_GRAYSCALE)
img02 = cv2.imread("data/goldengate-02.png", cv2.IMREAD_GRAYSCALE)
img03 = cv2.imread("data/goldengate-03.png", cv2.IMREAD_GRAYSCALE)
img04 = cv2.imread("data/goldengate-04.png", cv2.IMREAD_GRAYSCALE)
img05 = cv2.imread("data/goldengate-05.png", cv2.IMREAD_GRAYSCALE)

sift = cv2.SIFT_create()

kp00, des00 = sift.detectAndCompute(img00, None)
kp01, des01 = sift.detectAndCompute(img01, None)
kp02, des02 = sift.detectAndCompute(img02, None)
kp03, des03 = sift.detectAndCompute(img03, None)
kp04, des04 = sift.detectAndCompute(img04, None)
kp05, des05 = sift.detectAndCompute(img05, None)

bf = cv2.BFMatcher()
matches01 = bf.knnMatch(des00, des01, k=2)
matches12 = bf.knnMatch(des01, des02, k=2)
matches23 = bf.knnMatch(des02, des03, k=2)
matches34 = bf.knnMatch(des03, des04, k=2)
matches45 = bf.knnMatch(des04, des05, k=2)

good_matches01 = []
for m, n in matches01:
   if m.distance < 0.6 * n.distance:
       good_matches01.append(m)

good_matches12 = []
for m, n in matches12:
   if m.distance < 0.6 * n.distance:
       good_matches12.append(m)

good_matches23 = []
for m, n in matches23:
   if m.distance < 0.6 * n.distance:
       good_matches23.append(m)

good_matches34 = []
for m, n in matches34:
   if m.distance < 0.6 * n.distance:
       good_matches34.append(m)

good_matches45 = []
for m, n in matches45:
   if m.distance < 0.6 * n.distance:
       good_matches45.append(m)

# Stitch function with OpenCV warp_perspective function
def stitch(img1, img2, H):
    rows1, cols1 = img1.shape[:2]
    rows2, cols2 = img2.shape[:2]

    list_of_points_1 = np.float32([[0, 0], [0, rows1], [cols1, rows1], [cols1, 0]]).reshape(-1, 1, 2)
    temp_points = np.float32([[0, 0], [0, rows2], [cols2, rows2], [cols2, 0]]).reshape(-1, 1, 2)

    # Warp perspective: change field of view
    list_of_points_2 = cv2.perspectiveTransform(temp_points, H)

    list_of_points = np.concatenate((list_of_points_1, list_of_points_2), axis=0)

    # Normalizing the distances of images that will be stitched:
    [x_min, y_min] = np.int32(list_of_points.min(axis=0).ravel() - 0.5)
    [x_max, y_max] = np.int32(list_of_points.max(axis=0).ravel() + 0.5)

    translation_dist = [-x_min, -y_min]

    H_translation = np.array([[1, 0, translation_dist[0]], [0, 1, translation_dist[1]], [0, 0, 1]])

    output_img = cv2.warpPerspective(img2, H_translation.dot(H), (x_max - x_min, y_max - y_min))
    output_img[translation_dist[1]:rows1 + translation_dist[1], translation_dist[0]:cols1 + translation_dist[0]] = img1

    return output_img

# I stitched the images one-by-one and with centered direction.

"""

-> -> ->|<- <- <-
00 01 02|03 04 05
  r0 02 | 03  r1
    r2  |   r3
     panorama  

"""

# * * * * * * * * * * * * * Direction: 00 -> 01 * * * * * * * * * * * * * * * * * * * * * * * * * * *

src_pts0 = np.float32([kp00[m.queryIdx].pt for m in good_matches01]).reshape(-1, 1, 2)
dst_pts0 = np.float32([kp01[m.trainIdx].pt for m in good_matches01]).reshape(-1, 1, 2)

M0, mask0 = cv2.findHomography(src_pts0, dst_pts0, cv2.RANSAC, 5.0)

result0 = stitch(img01, img00, M0)

# Detecting keypoints and descriptors of the new image:
kp_result0, des_result0 = sift.detectAndCompute(result0, None)

cv2.imwrite("data/result0.png", result0)

# * * * * * * * * * * * * * Direction: 04 <- 05 * * * * * * * * * * * * * * * * * * * * * * * * * * *

dst_pts1 = np.float32([kp04[m.queryIdx].pt for m in good_matches45]).reshape(-1, 1, 2)
src_pts1 = np.float32([kp05[m.trainIdx].pt for m in good_matches45]).reshape(-1, 1, 2)

M1, mask1 = cv2.findHomography(src_pts1, dst_pts1, cv2.RANSAC, 5.0)

result1 = stitch(img04, img05, M1)

kp_result1, des_result1 = sift.detectAndCompute(result1, None)

cv2.imwrite("data/result1.png", result1)

# * * * * * * * * * * * * * Direction: r0 -> 02 * * * * * * * * * * * * * * * * * * * * * * * * * * *

# Calculating matches between new Image and image_03
matches02_r0 = bf.knnMatch(des02, des_result0, k=2)

# Ratio tests for new match.
good_matches02_r0 = []
for m, n in matches02_r0:
   if m.distance < 0.6 * n.distance:
       good_matches02_r0.append(m)

src_pts2 = np.float32([kp02[m.queryIdx].pt for m in good_matches02_r0]).reshape(-1, 1, 2)
dst_pts2 = np.float32([kp_result0[m.trainIdx].pt for m in good_matches02_r0]).reshape(-1, 1, 2)

M2, mask2 = cv2.findHomography(dst_pts2, src_pts2, cv2.RANSAC, 5.0)

result2 = stitch(img02, result0, M2)

kp_result2, des_result2 = sift.detectAndCompute(result2, None)

cv2.imwrite("data/result2.png", result2)

# * * * * * * * * * * * * * Direction: 03 <- r1 * * * * * * * * * * * * * * * * * * * * * * * * * * *

matches03_r1 = bf.knnMatch(des03, des_result1, k=2)

good_matches03_r1 = []
for m, n in matches03_r1:
   if m.distance < 0.6 * n.distance:
       good_matches03_r1.append(m)

dst_pts3 = np.float32([kp03[m.queryIdx].pt for m in good_matches03_r1]).reshape(-1, 1, 2)
src_pts3 = np.float32([kp_result1[m.trainIdx].pt for m in good_matches03_r1]).reshape(-1, 1, 2)

M3, mask3 = cv2.findHomography(src_pts3, dst_pts3, cv2.RANSAC, 5.0)

result3 = stitch(img03, result1, M3)

kp_result3, des_result3 = sift.detectAndCompute(result3, None)

cv2.imwrite("data/result3.png", result3)

# Final stitching that will creates panorama:

# * * * * * * * * * * * * * Direction: r2 -> r3 * * * * * * * * * * * * * * * * * * * * * * * * * * *

matches_r2_r3 = bf.knnMatch(des_result3, des_result2, k=2)

good_matches_r2_r3 = []
for m, n in matches_r2_r3:
   if m.distance < 0.6 * n.distance:
       good_matches_r2_r3.append(m)

src_pts4 = np.float32([kp_result2[m.trainIdx].pt for m in good_matches_r2_r3]).reshape(-1, 1, 2)
dst_pts4 = np.float32([kp_result3[m.queryIdx].pt for m in good_matches_r2_r3]).reshape(-1, 1, 2)

M4, mask4 = cv2.findHomography(dst_pts4, src_pts4, cv2.RANSAC, 5.0)

result4 = stitch(result2, result3, M4)

cv2.imwrite("data/panorama.png", result4)

# * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
