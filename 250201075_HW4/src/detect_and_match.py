# Yaşar Polatlı
# 250201075

import numpy as np
#import cv2
from matplotlib import pyplot as plt
from cv2 import cv2

# Reading images from directory.
img00 = cv2.imread("data/goldengate-00.png", cv2.IMREAD_GRAYSCALE)
img01 = cv2.imread("data/goldengate-01.png", cv2.IMREAD_GRAYSCALE)
img02 = cv2.imread("data/goldengate-02.png", cv2.IMREAD_GRAYSCALE)
img03 = cv2.imread("data/goldengate-03.png", cv2.IMREAD_GRAYSCALE)
img04 = cv2.imread("data/goldengate-04.png", cv2.IMREAD_GRAYSCALE)
img05 = cv2.imread("data/goldengate-05.png", cv2.IMREAD_GRAYSCALE)

sift = cv2.SIFT_create()

# Detecting SIFT interest points and descriptors.
kp00, des00 = sift.detectAndCompute(img00, None)
kp01, des01 = sift.detectAndCompute(img01, None)
kp02, des02 = sift.detectAndCompute(img02, None)
kp03, des03 = sift.detectAndCompute(img03, None)
kp04, des04 = sift.detectAndCompute(img04, None)
kp05, des05 = sift.detectAndCompute(img05, None)

# Calculating SIFT descriptor matches between consecutive pairs of images by brute force matching.
bf = cv2.BFMatcher()
matches01 = bf.knnMatch(des00, des01, k=2)
matches12 = bf.knnMatch(des01, des02, k=2)
matches23 = bf.knnMatch(des02, des03, k=2)
matches34 = bf.knnMatch(des03, des04, k=2)
matches45 = bf.knnMatch(des04, des05, k=2)

# Ratio tests for each matches.
good_matches01 = []
for m, n in matches01:
   if m.distance < 0.6 * n.distance:
       good_matches01.append([m])

good_matches12 = []
for m, n in matches12:
   if m.distance < 0.6 * n.distance:
       good_matches12.append([m])

good_matches23 = []
for m, n in matches23:
   if m.distance < 0.6 * n.distance:
       good_matches23.append([m])

good_matches34 = []
for m, n in matches34:
   if m.distance < 0.6 * n.distance:
       good_matches34.append([m])

good_matches45 = []
for m, n in matches45:
   if m.distance < 0.6 * n.distance:
       good_matches45.append([m])

# Drawing these tentative correspondences on a match (tc_i-j) image:
tc_01 = cv2.drawMatchesKnn(img00, kp00, img01, kp01, good_matches01, None, (208, 224, 64), flags=2)
tc_12 = cv2.drawMatchesKnn(img01, kp01, img02, kp02, good_matches12, None, (208, 224, 64), flags=2)
tc_23 = cv2.drawMatchesKnn(img02, kp02, img03, kp03, good_matches23, None, (208, 224, 64), flags=2)
tc_34 = cv2.drawMatchesKnn(img03, kp03, img04, kp04, good_matches34, None, (208, 224, 64), flags=2)
tc_45 = cv2.drawMatchesKnn(img04, kp04, img05, kp05, good_matches45, None, (208, 224, 64), flags=2)

# Drawing the SIFT interest points on each image:
img00 = cv2.drawKeypoints(img00, kp00, None, (208, 224, 64), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
img01 = cv2.drawKeypoints(img01, kp01, None, (208, 224, 64), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
img02 = cv2.drawKeypoints(img02, kp02, None, (208, 224, 64), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
img03 = cv2.drawKeypoints(img03, kp03, None, (208, 224, 64), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
img04 = cv2.drawKeypoints(img04, kp04, None, (208, 224, 64), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)
img05 = cv2.drawKeypoints(img05, kp05, None, (208, 224, 64), flags=cv2.DRAW_MATCHES_FLAGS_DRAW_RICH_KEYPOINTS)

# Saving resulting images:
cv2.imwrite("data/tentative_correspondences_0-1.png", tc_01)
cv2.imwrite("data/tentative_correspondences_1-2.png", tc_12)
cv2.imwrite("data/tentative_correspondences_2-3.png", tc_23)
cv2.imwrite("data/tentative_correspondences_3-4.png", tc_34)
cv2.imwrite("data/tentative_correspondences_4-5.png", tc_45)

cv2.imwrite("data/sift_keypoints_00.png", img00)
cv2.imwrite("data/sift_keypoints_01.png", img01)
cv2.imwrite("data/sift_keypoints_02.png", img02)
cv2.imwrite("data/sift_keypoints_03.png", img03)
cv2.imwrite("data/sift_keypoints_04.png", img04)
cv2.imwrite("data/sift_keypoints_05.png", img05)

# Saving the SIFT interest points and descriptors as text files in the same folder with names as sift_i.txt
for i in range(6):
    with open("data/sift_"+str(i)+".txt", "w") as output:
        dumb_list = [kp00, kp01, kp02, kp03, kp04, kp05]
        dumb_list2 = [des00, des01, des02, des03, des04, des05]
        for j in range(len(dumb_list[i])):
            output.write("{Keypoint %i} Coordinates: %s Orientation: %s Pyramid (DoG) Level: %s Diameter: %s \n"
                         % (j, dumb_list[i][j].pt, dumb_list[i][j].angle, dumb_list[i][j].octave, dumb_list[i][j].size))
        for k in range(len(dumb_list2[i])):
            output.write("{Descriptor %i} %s\n"
                         % (k, dumb_list2[i][k]))

# Saving the tentative correspondences as text files in the same folder with names as tentative_correspondences_i-j.txt
dumb_list3 = ["0-1", "1-2", "2-3", "3-4", "4-5"]
for i in range(5):
    with open("data/tentative_correspondences_"+dumb_list3[i]+".txt", "w") as output:
        dumb_list4 = [good_matches01, good_matches12, good_matches23, good_matches34, good_matches45]
        for j in range(len(dumb_list4[i])):
            output.write("DMatch %i: %s \n"
                         % (j, dumb_list4[i][j]))
