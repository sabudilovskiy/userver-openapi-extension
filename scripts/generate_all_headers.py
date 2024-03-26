import os
import sys


def path(prefix, root_directory, path):
    relative = os.path.relpath(path, root_directory)
    return prefix + '/' + relative

# Функция для создания файла all.hpp
def create_all_hpp(prefix_path, folder_path, root):
    hpp_files = [f for f in os.listdir(folder_path) if (f.endswith(".hpp") and f != "all.hpp")]

    with open(os.path.join(folder_path, "all.hpp"), "w") as all_hpp_file:
        all_hpp_file.write('#pragma once\n')
        for hpp_file in hpp_files:
            path_hpp_file = path(prefix_path, root, os.path.join(folder_path, hpp_file))
            all_hpp_file.write(f'#include <{path_hpp_file}>\n')

        # Поиск вложенных папок
        sub_folders = [f for f in os.listdir(folder_path) if os.path.isdir(os.path.join(folder_path, f))]

        for sub_folder in sub_folders:
            path_sub_folder = path(prefix_path, root, os.path.join(folder_path, sub_folder))
            all_hpp_file.write(f'#include <{path_sub_folder}/all.hpp>\n')


def create_all_hpp_in_directory(prefix_path, root):
    for folder_name, sub_folders, filenames in os.walk(root):
        create_all_hpp(prefix_path, folder_name, root)

def main():
    if len(sys.argv) < 2:
        print('Usage: python3 generate_sql.py <directory> <prefix_path>')
        return

    directory = sys.argv[1]
    if len(sys.argv) >= 3:
        prefix_path = sys.argv[2]
    else:
        prefix_path = ''

    create_all_hpp_in_directory(prefix_path, directory)


if __name__ == '__main__':
    main()
