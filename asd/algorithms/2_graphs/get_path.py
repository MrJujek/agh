def get_path(parents, start_node, target_node):
    path = []
    current = target_node

    while current is not None and current != start_node:
        path.append(current)
        current = parents[current]

    if current == start_node:
        path.append(start_node)
        return path[::-1]
    else:
        return None
    
parents = [None, 3, 0, 0, 1]
print(get_path(parents, 0, 4))