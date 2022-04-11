# cpp_contours
Mini (updated) version of gnhm/cpp-contour-tracker for version control, not forked as I deleted a bunch of files that I didn't need and was easier to just reupload


## usage
`./track_movie <file_path>/<filename>.movie <centre_x> <centre_y> <first_point_x> <first_point_y>`


## test data
available at https://drive.google.com/drive/folders/1_QW4JKfIoXBQNUl5Ca2oFm783Q6HX-Oi?usp=sharing
- the BF_2 is a "clean" image, I used centre=(84,58), first_point=(105,49), and it seemed to give some good frames but still ran off to zero a few times
- the BF_4 has more stuff surrounding it which is annoying, I used centre=(93, 57), first_point=(63, 58) and it also had a few frames where it just decided to go to zero midway through
