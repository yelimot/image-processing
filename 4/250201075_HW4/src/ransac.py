# Yaşar Polatlı
# 250201075

import numpy as np
#import cv2
from matplotlib import pyplot as plt
from cv2 import cv2

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

# I implement all the necessary work into this function and it does them by taking parameters for each image.
def RANSAC(matches, naming, img1, img2, kp1, kp2):
    src_pts = np.float32([kp1[m.queryIdx].pt for m in matches]).reshape(-1, 1, 2) # query descriptor index
    dst_pts = np.float32([kp2[m.trainIdx].pt for m in matches]).reshape(-1, 1, 2) # train descriptor index

    # Matching keypoints and tentative correspondences by RANSAC.
    M, mask = cv2.findHomography(src_pts, dst_pts, cv2.RANSAC, 5.0)

    # Normalization and the final estimation over all inliers.
    matchesMask = mask.ravel().tolist()
    h, w = img1.shape
    pts = np.float32([[0, 0], [0, h-1], [w-1, h-1], [w-1, 0]]).reshape(-1, 1, 2)
    dst = cv2.perspectiveTransform(pts, M)

    draw_params = dict(matchColor=(0, 255, 0),
                       singlePointColor=None,
                       matchesMask=matchesMask, # inliers parameter
                       flags=2)

    # Drawing and saving the resulting final inlier correspondences in files in the
    # data folder with names as inliers_i-j.png and inliers_i-j.txt.
    img2 = cv2.drawMatches(img1, kp1, img2, kp2, matches, None, **draw_params)
    cv2.imwrite("data/inliers"+naming+".png", img2)

    with open("data/inliers_"+naming+".txt", "w") as output:
        output.write("Inliers:\n %s " % (matchesMask))

    # Saving the resulting homography matrices in files within the folder data
    # with names such as h_i-j.txt, where i and j are image numbers:
    with open("data/h_"+naming+".txt", "w") as output:
        output.write("Homography matrix:\n %s " % (M))

RANSAC(good_matches01, "0-1", img00, img01, kp00, kp01)
RANSAC(good_matches12, "1-2", img01, img02, kp01, kp02)
RANSAC(good_matches23, "2-3", img02, img03, kp02, kp03)
RANSAC(good_matches34, "3-4", img03, img04, kp03, kp04)
RANSAC(good_matches45, "4-5", img04, img05, kp04, kp05)





















