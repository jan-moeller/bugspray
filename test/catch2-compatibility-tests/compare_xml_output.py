import argparse
import re


def compute_diff(expected, actual):
    import difflib
    expected = expected.splitlines(1)
    actual = actual.splitlines(1)

    diff = difflib.unified_diff(expected, actual)

    return ''.join(diff)


parser = argparse.ArgumentParser(description='Compare output of bugspray with output of catch')
parser.add_argument('a', type=str, help='first file')
parser.add_argument('b', type=str, help='second file')

args = parser.parse_args()


def hide_run_specific_data(content):
    # application name and filenames are allowed to differ
    content = re.sub(r'<Catch2TestRun name=".*?"', '<Catch2TestRun name="program_name"', content)
    content = re.sub(r'filename=".*?"', 'filename="some_file"', content)
    return content


with open(args.a) as f:
    a_content = f.read()
a_content = hide_run_specific_data(a_content)

with open(args.b) as f:
    b_content = f.read()
b_content = hide_run_specific_data(b_content)

if a_content == b_content:
    exit(0)
else:
    print("Strings differ:")
    print(compute_diff(a_content, b_content))
    exit(1)
