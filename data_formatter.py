from pathlib import Path

out_folder = Path("data")

out_folder.mkdir(parents=True, exist_ok=True)
NODE_THRESHOLD = 183964077 / 3 #File is about 2x too large, so cut it down with some safety margin

for raw_filepath in Path("data_raw").iterdir():
    next_node = 0
    node_map = dict()
    with open(raw_filepath) as raw_file:
        print(f"Processing {raw_filepath}")
        with open(out_folder / raw_filepath.stem, "w") as out_file:
            start_copy = False;
            skip_next = False;
            for raw_line in raw_file:
                raw_line = raw_line.strip()
                if (start_copy):
                    if (skip_next):
                        skip_next = False
                        continue
                    if (raw_filepath.name == "AGATHA_2015.mtx"):
                        node1_str, node2_str = raw_line.split(" ")
                        node1 = int(node1_str)
                        node2  = int(node2_str)
                        if (node1 > NODE_THRESHOLD):
                            continue
                        elif (node2 > NODE_THRESHOLD):
                            break
                    elif raw_filepath.name == "rec-amz-Books.edges":
                        node1, node2, weights, timestamp = raw_line.split(",")
                    else:
                        node1, node2 = raw_line.split()
                    if (node1 not in node_map.keys()):
                        node_map[node1] = next_node
                        next_node += 1
                    if (node2 not in node_map.keys()):
                        node_map[node2] = next_node
                        next_node += 1
                    out_file.write(str(node_map[node1]) + " " + str(node_map[node2]) + "\n")
                elif not raw_line.startswith("%") and not raw_line.startswith("#"):
                    start_copy = True
                    if raw_filepath.suffix == ".mtx":
                        skip_next = True
