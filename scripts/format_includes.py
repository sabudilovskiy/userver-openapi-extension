import os
import sys

def check_line(line: str, words):
    index_include: int = line.find("#include")
    if index_include == -1:
        return False
    index_include += len('#include')
    line_after_include = line[index_include:]
    index: int = line_after_include.find("\"") + 1
    if (index == 0):
        return False
    file = line_after_include[index:]
    for word in words:
        if (file.startswith(word)):
            return True
    return False


def format_line(line: str):
    return line.replace("\"", "<", 1).replace("\"", ">", 1)



def format_file(input_path, words):
    with open(input_path, 'r') as file:
        content = file.readlines()

    formatted_content = []
    for line in content:
        if check_line(line, words):
            formatted_content.append(format_line(line))
        else:
            formatted_content.append(line)

    with open(input_path, 'w') as file:
        file.writelines(formatted_content)

def format_directory(directory, words):
    for root, _, files in os.walk(directory):
        for file_name in files:
            if file_name.endswith(('.hpp', '.cpp')):
                file_path = os.path.join(root, file_name)
                format_file(file_path, words)
def main():
    if len(sys.argv) < 3:
        print('Usage: python3 format_includes.py <directory> start_words...')
        return

    input_path = sys.argv[1]
    words = sys.argv[2:]
    format_directory(input_path, words)


if __name__ == '__main__':
    main()