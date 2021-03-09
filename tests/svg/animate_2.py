from xml.dom import minidom
from glob import glob
import time
import re


FPS = 15


def add_animation(dom, attr, values, duration):
    values = "; ".join(values).strip() + ";"
    if attr in ["translate", "scale"]:
        added = ""
        if attr == "scale":
            added = """additive="sum" """

        data = (f"""<animateTransform 
        attributeName="transform" 
        attributeType="XML" 
        type="{attr}" 
        dur="{duration}ms" 
        repeatCount="indefinite" 
        values="{values}" {added} />
        """)
    else:
        data = (f"""<animate 
        attributeName="{attr}" 
        dur="{duration}ms" 
        repeatCount="indefinite"
        values="{values}"/>
        """)
    dom.appendChild(minidom.parseString(data).documentElement)


start = time.time()
# determine n_frames and duration
n_frames = len(glob("render/*.svg"))
duration = int(n_frames / FPS * 1000)

# remove data
for i in range(1, n_frames):
    svg = open(f"render/{i}.svg").read()
    svg = re.sub(r"data:image/jpeg([^\"]+)", "", svg)
    with open(f"render/{i}.svg", "w") as f:
        f.write(svg)

# create final animated svg
with open("animated.svg", "w") as f:
    with open("render/0.svg") as f2:
        data = f2.read()
    f.write(data)

# =============================================================================
# get animations
# =============================================================================
use_anim = {"x": {}, "y": {}, "transform": {}}
path_anim = {"d": {}}
g_anim = {"fill-opacity": {}}
for i in range(n_frames):
    doc = minidom.parse(open(f"render/{i}.svg"))

    # uses animations
    uses = doc.getElementsByTagName("use")
    use_anim_keys = list(use_anim.keys())
    for j, use in enumerate(uses):
        for key in use_anim_keys:
            value = use.getAttribute(key)
            if len(value) == 0:
                continue
            if key == "transform":
                value = value[7:-1].split(",")
                if "translate" not in use_anim:
                    use_anim["translate"] = {}
                use_anim["translate"].setdefault(j, []).append(" ".join(value[4:]))

                if "scale" not in use_anim:
                    use_anim["scale"] = {}
                use_anim["scale"].setdefault(j, []).append(value[0])
            else:
                use_anim[key].setdefault(j, []).append(value)

    # paths animations
    paths = doc.getElementsByTagName("path")
    for j, path in enumerate(paths):
        for key in path_anim:
            path_anim[key].setdefault(j, []).append(path.getAttribute(key))
    
    # g's animations
    gs = doc.getElementsByTagName("g")
    for j, g in enumerate(gs):
        data = g.getAttribute("style").strip()
        if len(g.getAttribute("style")) == 0:
            continue
        data = data.split(";")
        for item in data:
            if len(item) == 0:
                continue
            attr, value = item.split(":")
            if attr not in g_anim:
                g_anim[attr] = {}
            g_anim[attr].setdefault(j, []).append(value)
    doc.unlink()


doc = minidom.parse(open(f"animated.svg"))

# uses animations
for i, use in enumerate(doc.getElementsByTagName("use")):
    for attr in use_anim:
        attr_values = use_anim[attr].get(i, "")
        if len(set(attr_values)) <= 1:
            continue
        add_animation(use, attr, attr_values, duration)

# paths animations
for i, path in enumerate(doc.getElementsByTagName("path")):
    for attr in path_anim:
        attr_values = path_anim[attr][i]
        if len(set(attr_values)) <= 1:
            continue
        add_animation(path, attr, attr_values, duration)

# gs animations
for i, g in enumerate(doc.getElementsByTagName("g")):
    for attr in g_anim:
        attr_values = g_anim[attr].get(i, "")
        if len(set(attr_values)) <= 1:
            continue
        add_animation(g, attr, attr_values, duration)


with open("animated.svg", "w") as f:
    doc.writexml(f)
doc.unlink()
print(time.time() - start)