from cmotion.animation import Timeline
from cmotion.shape import Rectangle
from cmotion.media import Image
from cmotion.text import Text
from cmotion import Scene
from ffmpeg import frames_to_video
import time

# global variables
FPS = 30
WIDTH = 540 // 2
HEIGHT = 960 // 2
# WIDTH = 1080
# HEIGHT = 1920
MARGIN_X = 2
MARGIN_Y = 1
TRANSITION = .4
DURATION = 4
WORD_TRANSITION = .1
KEYFRAMES = [0, TRANSITION, TRANSITION + DURATION, 2*TRANSITION + DURATION]

# create scene object
scene = Scene(WIDTH, HEIGHT)


# =============================================================================
# title
# =============================================================================
tl = Timeline(KEYFRAMES)
tl.cubic_out("x", -5, 0)
tl.constant("x", 0)
tl.cubic_in("x", 0, 5)
tl.cubic_out("opacity", 0, 1)
tl.constant("opacity", 1)
tl.cubic_in("opacity", 1, 0)

category = Text("TRAVEL",
                font_family="../../fonts/Oswald-Bold.ttf",
                color="#000000", z_index=2, size=3)
category.set_position(50, 10)
category.animate_words(tl, WORD_TRANSITION)
scene.add(category)

# highlight
c_bbox = category.get_lines_bbox()[0]
c_highlight = Rectangle(width=c_bbox["width"] + 2 * MARGIN_X,
                        height=c_bbox["height"] + 2 * MARGIN_Y,
                        x=c_bbox["x"] - MARGIN_X + .1,
                        y=c_bbox["y"] - MARGIN_Y,
                        align="top_left",
                        color="#89f4ab", z_index=1)
scene.add(c_highlight)


# =============================================================================
# Title
# =============================================================================
title = Text("Discover our top 10 Top Travel Spots \nfor 2021",
             font_family="../../fonts/NunitoSans-Bold.ttf",
             align="top_left", space_size=0.3, line_height=1.1,
             width=80, color="#000000", z_index=2, size=8)
title.set_position(10, 50)
title.animate_words(tl, WORD_TRANSITION)
scene.add(title)

# highlight
t_bbox = title.get_lines_bbox()
t_highlights = []
for i, bbox in enumerate(t_bbox):
    x = bbox["x"] - 1.5 * MARGIN_X + .1
    y = bbox["y"] - 1.5 * MARGIN_Y
    width = bbox["width"] + 3 * MARGIN_X
    height = bbox["height"] + 3 * MARGIN_Y

    tl_highlight = Timeline(KEYFRAMES, delay=2 * i * WORD_TRANSITION)
    tl_highlight.cubic_out("width", 0, width)
    tl_highlight.constant("width", width)
    tl_highlight.constant("width", width)

    tl_highlight.constant("height", height)
    tl_highlight.constant("height", height)
    tl_highlight.cubic_in("height", height, 0)

    highlight = Rectangle(
        width=width,
        height=height,
        x=x, y=y,
        align="top_left",
        color="#FFFFFF", z_index=1)
    highlight.add(tl_highlight)
    scene.add(highlight)
    t_highlights.append(highlight)


# =============================================================================
# Images
# =============================================================================
# image 2
tl_img = Timeline([0, title.get_end()])
tl_img.linear("scale", 1, 1.2)

tl_img2 = Timeline([0, DURATION // 2 - TRANSITION, DURATION // 2 + 3 * TRANSITION, title.get_end()])
tl_img2.constant("height", 100)
tl_img2.cubic_in_out("height", 100, 50)
tl_img2.constant("height", 50)
tl_img2.constant("y", 50)
tl_img2.cubic_in_out("y", 50, 0)
tl_img2.constant("y", 0)

image = Image("img/beach.png", width=100, height=100, fit="cover", align="center", z_index=0)
image.set_position(50, 50)
image.add(tl_img)
image.add(tl_img2)
scene.add(image)

# image 2
tl_img3 = Timeline([0, title.get_end()])
tl_img3.linear("scale", 1, 1.2)

tl_img4 = Timeline([0, DURATION // 2 - TRANSITION, DURATION // 2 + 3 * TRANSITION, title.get_end()])
tl_img4.constant("y", 100)
tl_img4.cubic_in_out("y", 100, 75)
tl_img4.constant("y", 75)


image_2 = Image("img/forest.png", width=100, height=100, fit="cover", z_index=-1)
image_2.set_position(50, 100)
image_2.add(tl_img3)
image_2.add(tl_img4)
scene.add(image_2)

# =============================================================================
# Rendering
# =============================================================================
scene.save("test.jpg", .2)

start = time.time()
scene.render("render", fps=FPS)
print(time.time() - start)
frames_to_video("render", "render.mp4", frame_rate=FPS)
print(time.time() - start)
