from xml.dom import minidom
from glob import glob


def build_path(fps=30):
    path_d = {}
    n_frames = len(glob("render/*.svg"))
    for i in range(n_frames):
        doc = minidom.parse(open(f"render/{i}.svg"))
        surface = doc.getElementsByTagName('g')[0]

        paths = surface.getElementsByTagName("path")
        for j, path in enumerate(paths):
            path_d.setdefault(j, []).append(path.getAttribute("d"))
        doc.unlink()
    duration = int(n_frames / fps * 1000)
    return path_d, duration


def to_animation(path, duration):
    data = "; ".join(path).strip() + ";"
    return (f"""
<animate 
    attributeName="d" 
    dur="{duration}ms" 
    repeatCount="indefinite"
    values="{data}"/>
    """)


with open("animated.svg", "w") as f:
    with open("render/0.svg") as f2:
        data = f2.read()
    f.write(data)

doc = minidom.parse(open(f"animated.svg"))
surface = doc.getElementsByTagName('g')[0]
paths = surface.getElementsByTagName("path")
path_d, duration = build_path()
for j, path in enumerate(paths):
    inner = minidom.parseString(to_animation(path_d[j], duration))
    path.appendChild(inner.documentElement)
with open("animated.svg", "w") as f:
    doc.writexml(f)
doc.unlink()
