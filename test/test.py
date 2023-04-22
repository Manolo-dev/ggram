import time

def waiting(message:str) -> None:
    a = "⠇ ⡆ ⣄ ⣀⡀⢀⣀ ⣠ ⢰ ⠸ ⠙⠈⠉⠉⠁⠋ "
    i = 0
    print("╭" + "─" * (5 + len(message)) + "╮")
    while 1:
        print("│ \033[32m" + a[i % len(a)] + a[(i + 1) % len(a)] + "\033[0m ", message, " │", sep="")
        print("╰" + "─" * (5 + len(message)) + "╯")
        time.sleep(0.1)
        print("\033[2A\033[2K", end="")
        i += 2
    print()

waiting("test")