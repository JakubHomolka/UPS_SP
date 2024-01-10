import re


def validate_name(name):
    pattern = re.compile(r'^[a-z]+$')
    return bool(pattern.match(name))


def validate_ip(ip):
    pattern = re.compile(r'^\d{1,3}\.\d{1,3}\.\d{1,3}\.\d{1,3}$')

    if pattern.match(ip) or ip.lower() == 'localhost':
        return True
    else:
        return False


def validate_port(port):
    pattern = re.compile(r'^[1-9]\d*$')
    port_string = str(port)
    return bool(pattern.match(port_string))
