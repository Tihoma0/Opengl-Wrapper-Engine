import os


dir = "./";

files = os.walk(dir);

for root, dirs, files in files:
    for file in files:
        # print(file);
        if (file.endswith(".cpp")):
            with open(os.path.join(root, file), 'r') as f:
                content = f.read();
                if len(content) < 130:
                    doth_path = file.replace(".cpp", ".h");
                    print(doth_path);
                    with open(os.path.join(root, doth_path), 'r') as f2:
                        content2 = f2.read();
                        with open(os.path.join(root, file), 'w') as f3:
                            f3.write(content2);