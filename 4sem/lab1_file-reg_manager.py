import os
import shutil
import winreg
import argparse


# FILES

def file_create(s):
    try:
        f = open(s, 'w')
        f.close()
    except PermissionError:
        print("Permission error")
    except FileExistsError:
        print('Such file already exists')
    except:
        print("Another Error")


def file_remove(s):
    try:
        os.remove(s)
    except PermissionError:
        print("Permission error")
    except FileNotFoundError:
        print('Such file was not found')
    except:
        print("Another Error")


def file_write(s):
    try:
        with open(s, 'a') as f:
            f.write(input())  # enter?
    except PermissionError:
        print("Permission error")
    except FileNotFoundError:
        print('Such file was not found')
    except:
        print("Error")


def file_read(s):
    try:
        with open(s, 'r') as f:
            print(f.read())
    except PermissionError:
        print("Permission error")
    except FileNotFoundError:
        print('Such file was not found')
    except:
        print("Error")


def file_copy(s, d):
    try:
        shutil.copy(s, d)
    except PermissionError:
        print("Permission error")
    except FileNotFoundError:
        print('Such file was not found')
    except:
        print("Error")


def file_rename(s1, s2):
    try:
        os.rename(s1, s2)
    except PermissionError:
        print("Permission error")
    except FileNotFoundError:
        print('Such file was not found')
    except:
        print("Error")


# REG

def reg_create(key, name):
    try:
        match key:
            case "HKEY_CURRENT_USER":
                winreg.CreateKey(winreg.HKEY_CURRENT_USER, name)
            case "HKEY_LOCAL_MACHINE":
                winreg.CreateKey(winreg.HKEY_LOCAL_MACHINE, name)
    except PermissionError:
        print("Permission error")
    except WindowsError:
        print("Windows error: incorrect input")


def reg_delete(key, name):
    try:
        match key:
            case "HKEY_CURRENT_USER":
                winreg.DeleteKey(winreg.HKEY_CURRENT_USER, name)
            case "HKEY_LOCAL_MACHINE":
                winreg.DeleteKey(winreg.HKEY_LOCAL_MACHINE, name)
    except PermissionError:
        print("Permission error")
    except FileNotFoundError:
        print('Such file was not found')
    except WindowsError:
        print("Windows error: incorrect input")
    except:
        print("Error")


def reg_write(key, name, value):
    try:
        match key:
            case "HKEY_CURRENT_USER":
                winreg.SetValue(winreg.HKEY_CURRENT_USER, name, winreg.REG_SZ, value)
            case "HKEY_LOCAL_MACHINE":
                winreg.SetValue(winreg.HKEY_LOCAL_MACHINE, name, winreg.REG_SZ, value)
    except FileNotFoundError:
        print('Such file was not found')
    except PermissionError:
        print("Permission error")
    except WindowsError:
        print("Windows error: incorrect input")
    except:
        print("Error")


def parsing():
    parser = argparse.ArgumentParser()
    parser.add_argument('--file_create', type=str, metavar='FILE1', help='Creates a file', nargs=1, dest='create')
    parser.add_argument('--file_remove', type=str, metavar='FILE1', help='Removes a file', nargs=1, dest='remove')
    parser.add_argument('--file_write', type=str, metavar='FILE1', help='Writes the following text to the file',
                        nargs=1, dest='write')
    parser.add_argument('--file_read', type=str, metavar='FILE1', help='Displays text from a file',
                        nargs=1, dest='read')
    parser.add_argument('--file_copy', type=str, metavar=('FILE1', 'FILE2'), help='Copies text from a file',
                        nargs=2, dest='copy')
    parser.add_argument('--file_rename', type=str, metavar=('FILE1', 'FILE2'), help='Renames a file',
                        nargs=2, dest='rename')

    parser.add_argument('--reg_create', type=str, metavar=('KEY', 'NAME'), help='Creates a key', nargs=2,
                        dest='regcreate')
    parser.add_argument('--reg_delete', type=str, metavar=('KEY', 'NAME'), help='Removes a key', nargs=2,
                        dest='regdelete')
    parser.add_argument('--reg_write', type=str, metavar=('KEY', 'NAME', 'VALUE'), help='Inputs value in a key',
                        nargs=3, dest='regwrite')

    args = parser.parse_args()

    if args.create:
        file_create(args.create[0])
    elif args.remove:
        file_remove(args.remove[0])
    elif args.write:
        file_write(args.write[0])
    elif args.read:
        file_read(args.read[0])
    elif args.copy:
        file_copy(args.copy[0], args.copy[1])
    elif args.rename:
        file_rename(args.rename[0], args.rename[1])
    elif args.regcreate:
        reg_create(args.regcreate[0], args.regcreate[1])
    elif args.regdelete:
        reg_delete(args.regdelete[0], args.regdelete[1])
    elif args.regwrite:
        reg_write(args.regwrite[0], args.regwrite[1], args.regwrite[2])


if __name__ == "__main__":
    parsing()

