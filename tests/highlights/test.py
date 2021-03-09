from cmotion.shape import Circle, Rectangle
from cmotion.media import Image
from cmotion.text import Text
from cmotion.animation import Timeline
from ffmpeg import frames_to_video
from cmotion import Scene
import subprocess
import time
import os

subprocess.call("rm -rf results/", shell=True)
os.makedirs("results")


scene = Scene(1000, 1000)

# create text
tl = Timeline([0., .6, 5.4, 6, 6.2])
tl.cubic_out("opacity", 0, 1)
tl.constant("opacity", 1)
tl.cubic_in("opacity", 1, 0)
tl.constant("opacity", 0)

tl.cubic_out("y", 3, 0)
tl.constant("y", 0)
tl.cubic_in("y", 0, 3)
tl.constant("y", 3)

text = Text("Building wheels for collected packages: cmotion", align="center", size=6, width=90, z_index=1)
text.set_position(50, 50)
text.animate_lines(tl, .2)
scene.add(text)


lines_bbox = text.get_lines_bbox()
rects = []
for line_bbox in lines_bbox:
    x = line_bbox["x"]
    y = line_bbox["y"]
    width = line_bbox["width"]
    height = line_bbox["height"]
    rect = Rectangle(x=x, y=y, width=width, height=height,
                     color="#000000", align="top_left", z_index=0)
    rect.set_position(x, y)
    scene.add(rect)
    rects.append(rect)


fps = 60
quality = 100
# scene.resize(500, 500)
# scene.to_svg("test.svg", .1)

scene.render("results", fps=fps, quality=quality, antialias=0)
frames_to_video("results", "render.mp4", frame_rate=fps)
