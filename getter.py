import re

def get_file_val(path: str) -> str:
    """This function need utf-8 encoding file"""
    try:
        return open(path, "r", encoding='utf-8').read()
    except:
        print(f'[ERR] open file `{path}`')
        return ''

def convert_code(code: str) -> str:
    return re.sub('[ \n\t]+', '', code)

def get_actions(code: str) -> list:
    return re.split(';', code)

class Code:
    def __init__(self):
        self.code = ''
        self.actions = []

    def load_file(self, path):
        self.code = get_file_val(path)

    def load_code(self, code):
        self.code = code

    def process_actions(self):
        self.actions = get_actions(convert_code(self.code))
