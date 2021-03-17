import networkx as nx


def draw_network(
    G, xy,
    width=1000, height=1000,
    margin=10,
    node_color="#7AD71F",
    node_size=.5,
    edge_color="#DDDDDD",
    edge_opacity=.2,
    edge_size=.2,
    font_color="#FFFFFF",
    font_size=2
):
    from cmotion import Scene
    from cmotion.shape import Circle, Line
    from cmotion.text import Text
    import numpy as np

    n_nodes = len(G.nodes)
    node2id = {node: i for i, node in enumerate(G.nodes)}

    if not isinstance(node_size, list):
        node_size = [node_size] * n_nodes
    if not isinstance(node_color, list):
        node_color = [node_color] * n_nodes

    font_family = "../highlights/OpenSans-Light.ttf"

    # adjust node positions to 100x100
    xy = np.array([
        xy[node] for node in G.nodes
    ])
    x_max, y_max = xy.max(axis=0)
    x_min, y_min = xy.min(axis=0)
    size = (100 - 2 * margin)
    xy[:, 0] = size * (xy[:, 0] - x_min) / (x_max - x_min) + margin
    xy[:, 1] = size * (xy[:, 1] - y_min) / (y_max - y_min) + margin

    # create scene
    scene = Scene(width, height)

    # draw nodes
    for i, (node, (x, y)) in enumerate(zip(G.nodes, xy)):
        circle = Circle(radius=node_size[i], color=node_color[i], z_index=1)
        circle.set_position(x, y)
        scene.add(circle)

        txt = Text(str(node), color=font_color, size=font_size, font_family=font_family, z_index=2)
        txt.set_position(x, y + 2)
        scene.add(txt)

    for src, tgt in G.edges:
        src_x, src_y = xy[node2id[src]]
        tgt_x, tgt_y = xy[node2id[tgt]]
        
        line = Line([src_x, tgt_x], [src_y, tgt_y],
                    color=edge_color,
                    stroke_width=edge_size,
                    opacity=edge_opacity,
                    z_index=0,)
        scene.add(line)
    return scene


if __name__ == "__main__":
    # G = nx.cycle_graph(5)
    G = nx.karate_club_graph()
    pos = nx.spring_layout(G)
    scene = draw_network(G, pos)
    scene.to_svg("graph.svg")
