from cmotion import Scene
from cmotion.shape import Circle, Rectangle
from cmotion.media import Image
from cmotion.text import Text
from cmotion.animation import Timeline
import numpy as np
import shutil
import os

import time

start = time.time()

shutil.rmtree("render")
os.makedirs("render")


scene = Scene(1000, 1000)

tl = Timeline([0, 1, 3, 4])
tl.cubic_in_out("opacity", .01, 1)
tl.constant("opacity", 1)
tl.cubic_in_out("opacity", 1, .01)
tl.cubic_in_out("y", 5, 0)
tl.constant("y", 0)
tl.cubic_in_out("y", 0, -5)
text = Text("salut comment ça va ?", color="#FFFFFF", font_family="../highlights/OpenSans-Light.ttf", size=5, width=100, align="bottom_left", z_index=1)
text.set_position(5, 95)
text.animate_words(tl, .05)
scene.add(text)


tl_rect = Timeline([0, 1, 3, 4])
tl_rect.cubic_in_out("height", 0.001, 5)
tl_rect.constant("height", 5)
tl_rect.cubic_in_out("height", 5, 0.001)
rect = Rectangle(color="#0000FF", width=2, z_index=0, align="bottom_left")
rect.set_position(2, 95)
rect.add(tl_rect)
scene.add(rect)

tl_image = Timeline([0, 1, 3, 4])
tl_image.cubic_in_out("height", 1, 50)
tl_image.constant("height", 50)
tl_image.cubic_in_out("height", 50, 1)
tl_image.cubic_in_out("width", 30, 50)
tl_image.constant("width", 50)
tl_image.cubic_in_out("width", 50, 30)
image = Image("../../templates/01/img/beach.jpg", fit="cover", width=50, z_index=-1)
image.set_position(50, 50)
image.add(tl_image)
scene.add(image)

fps = 15
# scene.render("render", fps=30)
duration = scene.get_end()
n_frames = int(round(duration * fps))
for i, t in enumerate(np.linspace(0, duration, n_frames)):
    scene.to_svg(f"render/{i}.svg", t)

print(time.time() - start)
