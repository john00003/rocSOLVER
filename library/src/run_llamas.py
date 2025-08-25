import sys
import os

def main(argv):
    verify = False
    if argv[2] == 'yes':
        verify = True

    args = ["larft-example"]

    for matrix_file, special_file in zip(os.listdir(argv[0]), os.listdir(argv[1])):
        args = ["larft-example", os.join(argv[0], matrix_file), os.path.join(argv[1], special_file)]
        if verify:
            args.append('yes')
        try:
            os.execv("larft-example", args)
        except OSError as e:
            print(f"Error executing {args}: {e}")

if __name__ == "__main__":
    # usage: python run_llamas.py <matrix_log_dir> <special_log_dir> [yes]
        # the third argument is optional; if it is 'yes', then the example will be ran in verify mode
    main(sys.argv[1:])