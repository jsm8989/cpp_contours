# cpp_contours
Mini (updated) version of gnhm/cpp-contour-tracker. Added some debugging options, moved functions to a smaller number of headers, and removed files that weren't needed for my purposes.


## usage
Compile to the executable e.g. `track_movie` (.exe if on windows)
`./track_movie <file_path>/<filename>.movie <centre_x> <centre_y> <first_point_x> <first_point_y>`
Should show progress iterating over frames, and then output `<file_path>/<filename>_contour_full.txt`, which can be used for the power spectrum analysis inside the `contour_fitter.py` functions.


## test data
available at https://drive.google.com/drive/folders/1_QW4JKfIoXBQNUl5Ca2oFm783Q6HX-Oi?usp=sharing
- the BF_2 is a "clean" image, I used centre=(84,58), first_point=(105,49), and it seemed to give some good frames but still ran off to zero a few times
- the BF_4 has more stuff surrounding it which is annoying, I used centre=(93, 57), first_point=(63, 58) and it also had a few frames where it just decided to go to zero midway through
also available from Guil at https://drive.google.com/drive/folders/1inBNFgTOp-YL6fGWqYMn7FJx0v070A3K
- for example with cell_0000 I used the points (110,110), (150, 110) and this did not run off to zero during the frames