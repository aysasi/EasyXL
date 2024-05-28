//
//  block32x32.h
//  JPEGDecoder
//
//  Created by Alejandro Ysasi Cillero on 24/4/24.
//

#ifndef block32x32_h
#define block32x32_h

int ZigZag32x32[32*32] = {
    0, 32, 1, 2, 33, 64, 96, 65, 34, 3, 4, 35, 66, 97, 128, 160, 129, 98, 67, 36, 5, 6, 37, 68, 99, 130, 161, 192, 224, 193, 162, 131,
    100, 69, 38, 7, 8, 39, 70, 101, 132, 163, 194, 225, 256, 288, 257, 226, 195, 164, 133, 102, 71, 40, 9, 10, 41, 72, 103, 134, 165, 196, 227, 258,
    289, 320, 352, 321, 290, 259, 228, 197, 166, 135, 104, 73, 42, 11, 12, 43, 74, 105, 136, 167, 198, 229, 260, 291, 322, 353, 384, 416, 385, 354, 323, 292,
    261, 230, 199, 168, 137, 106, 75, 44, 13, 14, 45, 76, 107, 138, 169, 200, 231, 262, 293, 324, 355, 386, 417, 448, 480, 449, 418, 387, 356, 325, 294, 263,
    232, 201, 170, 139, 108, 77, 46, 15, 16, 47, 78, 109, 140, 171, 202, 233, 264, 295, 326, 357, 388, 419, 450, 481, 512, 544, 513, 482, 451, 420, 389, 358,
    327, 296, 265, 234, 203, 172, 141, 110, 79, 48, 17, 18, 49, 80, 111, 142, 173, 204, 235, 266, 297, 328, 359, 390, 421, 452, 483, 514, 545, 576, 608, 577,
    546, 515, 484, 453, 422, 391, 360, 329, 298, 267, 236, 205, 174, 143, 112, 81, 50, 19, 20, 51, 82, 113, 144, 175, 206, 237, 268, 299, 330, 361, 392, 423,
    454, 485, 516, 547, 578, 609, 640, 672, 641, 610, 579, 548, 517, 486, 455, 424, 393, 362, 331, 300, 269, 238, 207, 176, 145, 114, 83, 52, 21, 22, 53, 84,
    115, 146, 177, 208, 239, 270, 301, 332, 363, 394, 425, 456, 487, 518, 549, 580, 611, 642, 673, 704, 736, 705, 674, 643, 612, 581, 550, 519, 488, 457, 426, 395,
    364, 333, 302, 271, 240, 209, 178, 147, 116, 85, 54, 23, 24, 55, 86, 117, 148, 179, 210, 241, 272, 303, 334, 365, 396, 427, 458, 489, 520, 551, 582, 613,
    644, 675, 706, 737, 768, 800, 769, 738, 707, 676, 645, 614, 583, 552, 521, 490, 459, 428, 397, 366, 335, 304, 273, 242, 211, 180, 149, 118, 87, 56, 25, 26,
    57, 88, 119, 150, 181, 212, 243, 274, 305, 336, 367, 398, 429, 460, 491, 522, 553, 584, 615, 646, 677, 708, 739, 770, 801, 832, 864, 833, 802, 771, 740, 709,
    678, 647, 616, 585, 554, 523, 492, 461, 430, 399, 368, 337, 306, 275, 244, 213, 182, 151, 120, 89, 58, 27, 28, 59, 90, 121, 152, 183, 214, 245, 276, 307,
    338, 369, 400, 431, 462, 493, 524, 555, 586, 617, 648, 679, 710, 741, 772, 803, 834, 865, 896, 928, 897, 866, 835, 804, 773, 742, 711, 680, 649, 618, 587, 556,
    525, 494, 463, 432, 401, 370, 339, 308, 277, 246, 215, 184, 153, 122, 91, 60, 29, 30, 61, 92, 123, 154, 185, 216, 247, 278, 309, 340, 371, 402, 433, 464,
    495, 526, 557, 588, 619, 650, 681, 712, 743, 774, 805, 836, 867, 898, 929, 960, 992, 961, 930, 899, 868, 837, 806, 775, 744, 713, 682, 651, 620, 589, 558, 527,
    496, 465, 434, 403, 372, 341, 310, 279, 248, 217, 186, 155, 124, 93, 62, 31, 63, 94, 125, 156, 187, 218, 249, 280, 311, 342, 373, 404, 435, 466, 497, 528,
    559, 590, 621, 652, 683, 714, 745, 776, 807, 838, 869, 900, 931, 962, 993, 994, 963, 932, 901, 870, 839, 808, 777, 746, 715, 684, 653, 622, 591, 560, 529, 498,
    467, 436, 405, 374, 343, 312, 281, 250, 219, 188, 157, 126, 95, 127, 158, 189, 220, 251, 282, 313, 344, 375, 406, 437, 468, 499, 530, 561, 592, 623, 654, 685,
    716, 747, 778, 809, 840, 871, 902, 933, 964, 995, 996, 965, 934, 903, 872, 841, 810, 779, 748, 717, 686, 655, 624, 593, 562, 531, 500, 469, 438, 407, 376, 345,
    314, 283, 252, 221, 190, 159, 191, 222, 253, 284, 315, 346, 377, 408, 439, 470, 501, 532, 563, 594, 625, 656, 687, 718, 749, 780, 811, 842, 873, 904, 935, 966,
    997, 998, 967, 936, 905, 874, 843, 812, 781, 750, 719, 688, 657, 626, 595, 564, 533, 502, 471, 440, 409, 378, 347, 316, 285, 254, 223, 255, 286, 317, 348, 379,
    410, 441, 472, 503, 534, 565, 596, 627, 658, 689, 720, 751, 782, 813, 844, 875, 906, 937, 968, 999, 1000, 969, 938, 907, 876, 845, 814, 783, 752, 721, 690, 659,
    628, 597, 566, 535, 504, 473, 442, 411, 380, 349, 318, 287, 319, 350, 381, 412, 443, 474, 505, 536, 567, 598, 629, 660, 691, 722, 753, 784, 815, 846, 877, 908,
    939, 970, 1001, 1002, 971, 940, 909, 878, 847, 816, 785, 754, 723, 692, 661, 630, 599, 568, 537, 506, 475, 444, 413, 382, 351, 383, 414, 445, 476, 507, 538, 569,
    600, 631, 662, 693, 724, 755, 786, 817, 848, 879, 910, 941, 972, 1003, 1004, 973, 942, 911, 880, 849, 818, 787, 756, 725, 694, 663, 632, 601, 570, 539, 508, 477,
    446, 415, 447, 478, 509, 540, 571, 602, 633, 664, 695, 726, 757, 788, 819, 850, 881, 912, 943, 974, 1005, 1006, 975, 944, 913, 882, 851, 820, 789, 758, 727, 696,
    665, 634, 603, 572, 541, 510, 479, 511, 542, 573, 604, 635, 666, 697, 728, 759, 790, 821, 852, 883, 914, 945, 976, 1007, 1008, 977, 946, 915, 884, 853, 822, 791,
    760, 729, 698, 667, 636, 605, 574, 543, 575, 606, 637, 668, 699, 730, 761, 792, 823, 854, 885, 916, 947, 978, 1009, 1010, 979, 948, 917, 886, 855, 824, 793, 762,
    731, 700, 669, 638, 607, 639, 670, 701, 732, 763, 794, 825, 856, 887, 918, 949, 980, 1011, 1012, 981, 950, 919, 888, 857, 826, 795, 764, 733, 702, 671, 703, 734,
    765, 796, 827, 858, 889, 920, 951, 982, 1013, 1014, 983, 952, 921, 890, 859, 828, 797, 766, 735, 767, 798, 829, 860, 891, 922, 953, 984, 1015, 1016, 985, 954, 923,
    892, 861, 830, 799, 831, 862, 893, 924, 955, 986, 1017, 1018, 987, 956, 925, 894, 863, 895, 926, 957, 988, 1019, 1020, 989, 958, 927, 959, 990, 1021, 1022, 991, 1023
};


const double IDCTValues32x32[32][32] = {
    {1, 1.4125100802019777, 1.4074037375263826, 1.3989068359730783, 1.3870398453221475, 1.371831354193494, 1.3533180011743526, 1.3315443865537255, 1.3065629648763766, 1.278433918575241, 1.2472250129866713, 1.2130114330978081, 1.1758756024193588, 1.1359069844201428, 1.0932018670017576, 1.0478631305325907, 1.0000000000000002, 0.9497277818777543, 0.8971675863426364, 0.8424460355094194, 0.7856949583871023, 0.72705107329128, 0.6666556584777468, 0.6046542117908009, 0.5411961001461971, 0.47643419969316125, 0.41052452752235735, 0.3436258658070505, 0.2758993792829431, 0.20750822698821159, 0.1386171691990917, 0.06939217050794086}, // k = 0
    {1, 1.3989068359730783, 1.3533180011743526, 1.278433918575241, 1.1758756024193588, 1.0478631305325907, 0.8971675863426364, 0.72705107329128, 0.5411961001461971, 0.3436258658070505, 0.1386171691990917, -0.06939217050794037, -0.27589937928294295, -0.4764341996931614, -0.6666556584777467, -0.8424460355094189, -1.0, -1.135906984420143, -1.247225012986671, -1.3315443865537253, -1.3870398453221475, -1.4125100802019777, -1.4074037375263826, -1.371831354193494, -1.3065629648763768, -1.2130114330978081, -1.0932018670017574, -0.9497277818777549, -0.7856949583871021, -0.6046542117908013, -0.4105245275223587, -0.20750822698821236}, // k = 1
    {1, 1.371831354193494, 1.2472250129866713, 1.0478631305325907, 0.7856949583871023, 0.47643419969316125, 0.1386171691990917, -0.20750822698821142, -0.541196100146197, -0.8424460355094193, -1.0932018670017576, -1.2784339185752407, -1.3870398453221475, -1.4125100802019777, -1.3533180011743529, -1.2130114330978081, -1.0000000000000002, -0.7270510732912803, -0.4105245275223575, -0.06939217050794072, 0.27589937928294306, 0.6046542117908008, 0.8971675863426355, 1.1359069844201422, 1.3065629648763764, 1.3989068359730783, 1.4074037375263824, 1.3315443865537258, 1.175875602419359, 0.9497277818777541, 0.666655658477747, 0.34362586580705085}, // k = 2
    {1, 1.3315443865537255, 1.0932018670017576, 0.72705107329128, 0.2758993792829431, -0.20750822698821142, -0.6666556584777467, -1.0478631305325903, -1.3065629648763766, -1.4125100802019777, -1.3533180011743529, -1.1359069844201433, -0.7856949583871021, -0.34362586580704946, 0.13861716919909076, 0.6046542117907997, 0.9999999999999998, 1.2784339185752411, 1.4074037375263826, 1.3718313541934943, 1.175875602419359, 0.8424460355094191, 0.41052452752235885, -0.06939217050794003, -0.5411961001461972, -0.9497277818777551, -1.2472250129866722, -1.3989068359730779, -1.3870398453221477, -1.2130114330978083, -0.897167586342638, -0.4764341996931626}, // k = 3
    {1, 1.278433918575241, 0.8971675863426364, 0.3436258658070505, -0.27589937928294295, -0.8424460355094193, -1.247225012986671, -1.4125100802019777, -1.3065629648763768, -0.9497277818777549, -0.4105245275223575, 0.20750822698821064, 0.7856949583871017, 1.213011433097808, 1.4074037375263826, 1.3315443865537258, 1.0000000000000002, 0.47643419969316125, -0.13861716919909056, -0.7270510732912797, -1.1758756024193586, -1.3989068359730783, -1.3533180011743533, -1.0478631305325914, -0.5411961001461981, 0.06939217050793985, 0.6666556584777461, 1.1359069844201426, 1.3870398453221475, 1.3718313541934937, 1.0932018670017585, 0.6046542117908018}, // k = 4
    {1, 1.2130114330978081, 0.6666556584777468, -0.06939217050794069, -0.7856949583871019, -1.278433918575241, -1.4074037375263826, -1.1359069844201428, -0.5411961001461978, 0.20750822698821186, 0.8971675863426365, 1.331544386553725, 1.3870398453221475, 1.0478631305325896, 0.4105245275223577, -0.3436258658070488, -0.9999999999999989, -1.3718313541934941, -1.3533180011743526, -0.9497277818777559, -0.27589937928294267, 0.47643419969316153, 1.093201867001756, 1.3989068359730779, 1.3065629648763766, 0.8424460355094194, 0.13861716919908929, -0.6046542117907981, -1.1758756024193584, -1.4125100802019777, -1.2472250129866729, -0.727051073291283}, // k = 5
    {1, 1.1359069844201428, 0.41052452752235735, -0.4764341996931611, -1.1758756024193588, -1.4125100802019777, -1.0932018670017578, -0.3436258658070507, 0.5411961001461973, 1.213011433097808, 1.4074037375263826, 1.0478631305325914, 0.2758993792829437, -0.6046542117908008, -1.247225012986671, -1.3989068359730785, -0.9999999999999997, -0.20750822698821145, 0.6666556584777461, 1.2784339185752405, 1.3870398453221477, 0.9497277818777542, 0.13861716919909428, -0.7270510732912794, -1.3065629648763761, -1.3718313541934937, -0.8971675863426365, -0.06939217050794141, 0.7856949583871012, 1.3315443865537249, 1.3533180011743535, 0.8424460355094217}, // k = 6
    {1, 1.0478631305325907, 0.1386171691990917, -0.8424460355094193, -1.3870398453221475, -1.2130114330978081, -0.4105245275223575, 0.6046542117908008, 1.3065629648763764, 1.3315443865537258, 0.666655658477747, -0.3436258658070488, -1.1758756024193586, -1.398906835973078, -0.8971675863426362, 0.06939217050793985, 0.9999999999999988, 1.4125100802019777, 1.0932018670017585, 0.20750822698821414, -0.7856949583871012, -1.3718313541934941, -1.2472250129866729, -0.47643419969316536, 0.5411961001461967, 1.2784339185752402, 1.3533180011743522, 0.7270510732912832, -0.27589937928294345, -1.1359069844201424, -1.4074037375263828, -0.9497277818777565}, // k = 7
    {1, 0.9497277818777543, -0.13861716919909153, -1.1359069844201426, -1.3870398453221475, -0.7270510732912803, 0.41052452752235696, 1.2784339185752407, 1.3065629648763764, 0.47643419969316125, -0.6666556584777463, -1.3718313541934934, -1.1758756024193593, -0.20750822698821145, 0.8971675863426353, 1.4125100802019774, 0.9999999999999997, -0.06939217050794219, -1.0932018670017576, -1.3989068359730785, -0.7856949583871028, 0.3436258658070532, 1.2472250129866695, 1.3315443865537255, 0.5411961001461986, -0.6046542117908024, -1.3533180011743529, -1.2130114330978086, -0.27589937928294583, 0.8424460355094198, 1.407403737526382, 1.047863130532592}, // k = 8
    {1, 0.8424460355094194, -0.41052452752235713, -1.3315443865537253, -1.175875602419359, -0.06939217050794072, 1.0932018670017571, 1.3718313541934941, 0.541196100146198, -0.7270510732912797, -1.4074037375263826, -0.9497277818777559, 0.2758993792829415, 1.2784339185752416, 1.2472250129866718, 0.20750822698821414, -0.9999999999999986, -1.3989068359730783, -0.6666556584777474, 0.604654211790798, 1.3870398453221475, 1.0478631305325918, -0.13861716919908737, -1.2130114330978075, -1.306562964876378, -0.34362586580705046, 0.8971675863426385, 1.4125100802019777, 0.7856949583871031, -0.4764341996931631, -1.3533180011743513, -1.1359069844201442}, // k = 9
    {1, 0.72705107329128, -0.6666556584777467, -1.4125100802019777, -0.7856949583871021, 0.6046542117908008, 1.4074037375263826, 0.8424460355094191, -0.5411961001461972, -1.3989068359730783, -0.8971675863426362, 0.47643419969315914, 1.3870398453221475, 0.9497277818777542, -0.4105245275223577, -1.3718313541934934, -0.9999999999999998, 0.3436258658070532, 1.3533180011743529, 1.0478631305325918, -0.27589937928294345, -1.3315443865537264, -1.0932018670017605, 0.20750822698820956, 1.3065629648763768, 1.135906984420144, -0.13861716919909203, -1.2784339185752402, -1.1758756024193584, 0.06939217050793882, 1.2472250129866693, 1.213011433097809}, // k = 10
    {1, 0.6046542117908009, -0.8971675863426362, -1.371831354193494, -0.2758993792829436, 1.135906984420143, 1.2472250129866713, -0.06939217050794003, -1.3065629648763761, -1.0478631305325914, 0.41052452752235785, 1.3989068359730779, 0.7856949583871027, -0.7270510732912815, -1.4074037375263826, -0.47643419969316536, 0.9999999999999986, 1.3315443865537255, 0.13861716919909461, -1.2130114330978075, -1.1758756024193584, 0.20750822698820956, 1.3533180011743513, 0.9497277818777567, -0.5411961001461962, -1.4125100802019777, -0.6666556584777437, 0.8424460355094153, 1.3870398453221482, 0.3436258658070461, -1.093201867001752, -1.2784339185752416}, // k = 11
    {1, 0.47643419969316125, -1.0932018670017576, -1.2130114330978081, 0.27589937928294306, 1.3989068359730783, 0.666655658477747, -0.9497277818777534, -1.3065629648763766, 0.06939217050793985, 1.3533180011743522, 0.8424460355094215, -0.7856949583871012, -1.3718313541934937, -0.13861716919909445, 1.2784339185752402, 1.0, -0.6046542117908024, -1.4074037375263828, -0.34362586580705534, 1.1758756024193568, 1.135906984420141, -0.41052452752235224, -1.4125100802019774, -0.5411961001461991, 1.04786313053259, 1.2472250129866709, -0.20750822698820412, -1.3870398453221464, -0.7270510732912839, 0.8971675863426342, 1.331544386553726}, // k = 12
    {1, 0.3436258658070505, -1.247225012986671, -0.9497277818777549, 0.7856949583871017, 1.3315443865537258, -0.13861716919909056, -1.3989068359730783, -0.5411961001461981, 1.1359069844201426, 1.0932018670017585, -0.6046542117907981, -1.3870398453221477, -0.06939217050794141, 1.3533180011743529, 0.7270510732912832, -0.9999999999999984, -1.2130114330978086, 0.4105245275223572, 1.4125100802019777, 0.275899379282946, -1.2784339185752402, -0.8971675863426409, 0.8424460355094153, 1.306562964876378, -0.20750822698821406, -1.4074037375263824, -0.4764341996931663, 1.1758756024193593, 1.0478631305325925, -0.6666556584777403, -1.3718313541934941}, // k = 13
    {1, 0.20750822698821159, -1.3533180011743526, -0.6046542117908013, 1.1758756024193588, 0.9497277818777549, -0.8971675863426354, -1.2130114330978083, 0.541196100146197, 1.3718313541934943, -0.13861716919909023, -1.4125100802019774, -0.2758993792829455, 1.3315443865537264, 0.6666556584777477, -1.1359069844201395, -1.0000000000000002, 0.8424460355094198, 1.247225012986673, -0.4764341996931583, -1.387039845322148, 0.06939217050794384, 1.407403737526382, 0.34362586580705584, -1.3065629648763748, -0.7270510732912839, 1.0932018670017616, 1.0478631305325925, -0.7856949583871001, -1.2784339185752418, 0.4105245275223466, 1.3989068359730783}, // k = 14
    {1, 0.06939217050794086, -1.4074037375263824, -0.20750822698821236, 1.3870398453221473, 0.34362586580705085, -1.3533180011743522, -0.4764341996931626, 1.3065629648763761, 0.6046542117908018, -1.2472250129866709, -0.727051073291283, 1.175875602419357, 0.8424460355094178, -1.0932018670017558, -0.9497277818777565, 0.9999999999999982, 1.0478631305325887, -0.8971675863426346, -1.1359069844201442, 0.7856949583871006, 1.213011433097809, -0.6666556584777407, -1.2784339185752416, 0.541196100146191, 1.331544386553726, -0.4105245275223612, -1.3718313541934941, 0.27589937928293734, 1.3989068359730783, -0.138617169199086, -1.4125100802019777}, // k = 15
    {1, -0.06939217050794069, -1.4074037375263826, 0.20750822698821186, 1.3870398453221475, -0.3436258658070488, -1.3533180011743526, 0.47643419969316153, 1.3065629648763766, -0.6046542117908004, -1.2472250129866729, 0.727051073291277, 1.1758756024193582, -0.8424460355094199, -1.0932018670017574, 0.9497277818777509, 1.0000000000000002, -1.0478631305325903, -0.8971675863426369, 1.135906984420139, 0.7856949583871077, -1.2130114330978097, -0.6666556584777529, 1.2784339185752378, 0.5411961001461949, -1.3315443865537262, -0.41052452752235585, 1.371831354193492, 0.2758993792829421, -1.3989068359730783, -0.13861716919910116, 1.4125100802019772}, // k = 16
    {1, -0.20750822698821142, -1.3533180011743529, 0.6046542117908008, 1.175875602419359, -0.9497277818777534, -0.8971675863426362, 1.2130114330978077, 0.5411961001461983, -1.3718313541934941, -0.13861716919909445, 1.4125100802019777, -0.27589937928294345, -1.3315443865537255, 0.6666556584777455, 1.135906984420144, -0.9999999999999981, -0.8424460355094182, 1.2472250129866718, 0.4764341996931614, -1.3870398453221464, -0.06939217050794244, 1.4074037375263833, -0.3436258658070471, -1.3065629648763764, 0.7270510732912845, 1.093201867001758, -1.047863130532586, -0.7856949583871042, 1.2784339185752418, 0.4105245275223612, -1.398906835973077}, // k = 17
    {1, -0.34362586580705035, -1.2472250129866713, 0.9497277818777545, 0.7856949583871023, -1.331544386553725, -0.1386171691990916, 1.3989068359730785, -0.5411961001461969, -1.1359069844201437, 1.0932018670017558, 0.6046542117908044, -1.3870398453221475, 0.06939217050794418, 1.3533180011743537, -0.7270510732912766, -1.0000000000000002, 1.2130114330978097, 0.4105245275223602, -1.4125100802019774, 0.2758993792829377, 1.2784339185752396, -0.8971675863426302, -0.8424460355094266, 1.3065629648763764, 0.20750822698821333, -1.407403737526382, 0.4764341996931572, 1.175875602419362, -1.0478631305325925, -0.6666556584777538, 1.3718313541934917}, // k = 18
    {1, -0.4764341996931611, -1.0932018670017578, 1.213011433097808, 0.2758993792829437, -1.3989068359730785, 0.6666556584777461, 0.9497277818777542, -1.3065629648763761, -0.06939217050794141, 1.3533180011743535, -0.8424460355094159, -0.7856949583871031, 1.3718313541934946, -0.13861716919909203, -1.2784339185752436, 0.9999999999999979, 0.6046542117907959, -1.4074037375263824, 0.3436258658070424, 1.1758756024193615, -1.1359069844201417, -0.4105245275223655, 1.4125100802019779, -0.5411961001461952, -1.0478631305325896, 1.2472250129866735, 0.2075082269882137, -1.3870398453221473, 0.7270510732912839, 0.8971675863426459, -1.3315443865537224}, // k = 19
    {1, -0.6046542117908005, -0.8971675863426368, 1.371831354193494, -0.27589937928294167, -1.1359069844201435, 1.2472250129866709, 0.06939217050794123, -1.3065629648763777, 1.0478631305325905, 0.4105245275223596, -1.398906835973079, 0.7856949583871009, 0.7270510732912792, -1.4074037375263824, 0.47643419969315814, 1.000000000000004, -1.3315443865537264, 0.1386171691990915, 1.2130114330978092, -1.1758756024193564, -0.2075082269882082, 1.3533180011743553, -0.9497277818777538, -0.5411961001462001, 1.4125100802019772, -0.6666556584777485, -0.8424460355094273, 1.387039845322147, -0.3436258658070461, -1.0932018670017618, 1.2784339185752371}, // k = 20
    {1, -0.72705107329128, -0.6666556584777469, 1.4125100802019777, -0.7856949583871016, -0.6046542117908016, 1.4074037375263826, -0.8424460355094182, -0.5411961001461985, 1.3989068359730783, -0.8971675863426347, -0.4764341996931657, 1.387039845322148, -0.9497277818777545, -0.41052452752236, 1.3718313541934952, -0.9999999999999978, -0.3436258658070511, 1.3533180011743524, -1.0478631305325863, -0.27589937928294683, 1.331544386553726, -1.0932018670017516, -0.2075082269882185, 1.3065629648763786, -1.1359069844201415, -0.1386171691990915, 1.2784339185752442, -1.1758756024193557, -0.06939217050794383, 1.2472250129866764, -1.2130114330978086}, // k = 21
    {1, -0.8424460355094193, -0.4105245275223575, 1.3315443865537258, -1.1758756024193586, 0.06939217050793985, 1.0932018670017585, -1.3718313541934941, 0.5411961001461967, 0.7270510732912832, -1.4074037375263828, 0.9497277818777509, 0.275899379282946, -1.2784339185752394, 1.2472250129866718, -0.20750822698820412, -1.0000000000000007, 1.3989068359730792, -0.6666556584777403, -0.60465421179081, 1.3870398453221482, -1.047863130532593, -0.13861716919910116, 1.2130114330978095, -1.3065629648763741, 0.3436258658070512, 0.8971675863426304, -1.4125100802019772, 0.7856949583871032, 0.4764341996931631, -1.353318001174357, 1.135906984420138}, // k = 22
    {1, -0.9497277818777545, -0.13861716919909126, 1.1359069844201433, -1.3870398453221477, 0.7270510732912796, 0.4105245275223592, -1.2784339185752411, 1.3065629648763768, -0.4764341996931586, -0.6666556584777478, 1.3718313541934952, -1.1758756024193568, 0.2075082269882142, 0.8971675863426372, -1.412510080201978, 1.0000000000000013, 0.06939217050793761, -1.0932018670017611, 1.3989068359730779, -0.7856949583870998, -0.34362586580705184, 1.247225012986676, -1.3315443865537226, 0.5411961001461901, 0.6046542117907971, -1.3533180011743513, 1.2130114330978063, -0.27589937928294106, -0.8424460355094198, 1.4074037375263846, -1.0478631305325918}, // k = 23
    {1, -1.0478631305325903, 0.13861716919909076, 0.8424460355094191, -1.3870398453221477, 1.2130114330978077, -0.4105245275223577, -0.6046542117907997, 1.3065629648763777, -1.3315443865537249, 0.6666556584777455, 0.3436258658070555, -1.1758756024193584, 1.3989068359730785, -0.8971675863426383, -0.06939217050794244, 1.0000000000000042, -1.4125100802019777, 1.093201867001755, -0.2075082269882034, -0.7856949583871042, 1.371831354193493, -1.2472250129866662, 0.47643419969315687, 0.5411961001461959, -1.2784339185752422, 1.3533180011743537, -0.7270510732912705, -0.2758993792829384, 1.1359069844201424, -1.4074037375263821, 0.9497277818777489}, // k = 24
    {1, -1.1359069844201426, 0.41052452752235696, 0.47643419969316125, -1.1758756024193593, 1.4125100802019777, -1.0932018670017576, 0.3436258658070483, 0.5411961001461986, -1.2130114330978086, 1.4074037375263824, -1.0478631305325867, 0.27589937928294295, 0.6046542117907959, -1.2472250129866733, 1.3989068359730779, -0.9999999999999977, 0.20750822698820856, 0.6666556584777489, -1.2784339185752418, 1.3870398453221473, -0.9497277818777534, 0.1386171691990805, 0.7270510732912894, -1.3065629648763768, 1.3718313541934941, -0.8971675863426447, 0.06939217050794176, 0.7856949583871093, -1.3315443865537249, 1.3533180011743504, -0.8424460355094053}, // k = 25
    {1, -1.213011433097808, 0.6666556584777464, 0.06939217050794089, -0.7856949583871025, 1.2784339185752411, -1.4074037375263824, 1.1359069844201426, -0.5411961001461966, -0.20750822698821217, 0.8971675863426368, -1.3315443865537275, 1.3870398453221473, -1.0478631305325934, 0.4105245275223566, 0.3436258658070562, -1.0000000000000007, 1.371831354193493, -1.3533180011743524, 0.9497277818777498, -0.2758993792829417, -0.4764341996931578, 1.093201867001765, -1.3989068359730794, 1.306562964876376, -0.842446035509422, 0.1386171691990998, 0.6046542117908069, -1.1758756024193597, 1.4125100802019774, -1.2472250129866658, 0.727051073291274}, // k = 26
    {1, -1.278433918575241, 0.8971675863426365, -0.3436258658070488, -0.27589937928294267, 0.8424460355094194, -1.2472250129866729, 1.4125100802019777, -1.3065629648763768, 0.9497277818777545, -0.410524527522357, -0.20750822698821747, 0.7856949583871077, -1.2130114330978066, 1.4074037375263824, -1.3315443865537226, 1.000000000000001, -0.47643419969316203, -0.13861716919909117, 0.727051073291289, -1.1758756024193593, 1.3989068359730785, -1.3533180011743493, 1.0478631305325823, -0.541196100146185, -0.06939217050793414, 0.6666556584777412, -1.1359069844201515, 1.3870398453221466, -1.371831354193495, 1.0932018670017472, -0.6046542117907856}, // k = 27
    {1, -1.3315443865537253, 1.0932018670017571, -0.7270510732912797, 0.2758993792829415, 0.20750822698821164, -0.6666556584777474, 1.0478631305325918, -1.306562964876378, 1.4125100802019777, -1.3533180011743526, 1.135906984420139, -0.7856949583871005, 0.3436258658070521, 0.13861716919909564, -0.60465421179081, 1.0000000000000044, -1.2784339185752418, 1.4074037375263824, -1.371831354193493, 1.1758756024193586, -0.8424460355094223, 0.4105245275223456, 0.06939217050793899, -0.541196100146201, 0.9497277818777473, -1.2472250129866695, 1.3989068359730787, -1.387039845322146, 1.2130114330978108, -0.8971675863426205, 0.4764341996931552}, // k = 28
    {1, -1.371831354193494, 1.247225012986671, -1.0478631305325907, 0.7856949583871013, -0.47643419969316136, 0.13861716919909256, 0.207508226988212, -0.5411961001461988, 0.842446035509422, -1.0932018670017574, 1.2784339185752436, -1.387039845322147, 1.4125100802019777, -1.3533180011743526, 1.2130114330978068, -0.9999999999999974, 0.7270510732912843, -0.4105245275223509, 0.06939217050793241, 0.2758993792829428, -0.6046542117908018, 0.8971675863426463, -1.1359069844201453, 1.306562964876375, -1.3989068359730794, 1.4074037375263826, -1.3315443865537222, 1.175875602419358, -0.9497277818777595, 0.6666556584777424, -0.3436258658070347}, // k = 29
    {1, -1.3989068359730783, 1.3533180011743526, -1.2784339185752407, 1.1758756024193586, -1.0478631305325907, 0.8971675863426349, -0.7270510732912813, 0.5411961001461965, -0.3436258658070478, 0.13861716919909187, 0.06939217050794728, -0.2758993792829465, 0.47643419969316175, -0.6666556584777442, 0.8424460355094229, -1.000000000000001, 1.1359069844201417, -1.2472250129866738, 1.3315443865537295, -1.3870398453221473, 1.4125100802019774, -1.4074037375263813, 1.3718313541934914, -1.306562964876374, 1.213011433097806, -1.093201867001757, 0.949727781877741, -0.7856949583871066, 0.6046542117908081, -0.41052452752234886, 0.2075082269882058}, // k = 30
    {1, -1.4125100802019777, 1.4074037375263826, -1.3989068359730783, 1.3870398453221475, -1.3718313541934941, 1.3533180011743529, -1.3315443865537249, 1.3065629648763768, -1.2784339185752402, 1.2472250129866718, -1.2130114330978043, 1.1758756024193593, -1.1359069844201417, 1.093201867001755, -1.047863130532586, 1.0000000000000007, -0.9497277818777534, 0.8971675863426334, -0.8424460355094143, 0.7856949583871032, -0.7270510732912877, 0.6666556584777346, -0.6046542117907863, 0.5411961001461985, -0.4764341996931604, 0.41052452752235424, -0.34362586580704474, 0.27589937928293495, -0.2075082269882209, 0.13861716919907843, -0.06939217050792515} // k = 31
};

#endif /* block32x32_h */
