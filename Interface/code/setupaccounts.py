import sys

def main():
    print("It seems as though you don't have your accounts set up, I'll get to doing that right now, I just need to ask you some questions :-)")
    keyamount = int(input("How many accounts do you have sir? > "))
    s_key = ""
    p_key = ""
    #name = ""

    dirnname = input("Please input the name and directory of the new file e.g ~/accounts.txt > ")
    f = open(dirnname, "w+")
    for i in range(keyamount):
        s_key = input("What is the Private Key for the account, sir? > ")
        p_key = input("What is the Public key for the account, sir? > ")
        #name = raw_input("What would you like to call this account, sir? > ")
        f.write(str(s_key) + "\n" + str(p_key) + "\n")
    f.close()
    print("Your accounts are all setup now sir, you can now run the program like this for example:")
    print("./main.py -a " + dirnname + "-t buy -o market -q 100 -c BTCUSDT")
    print("Have a good day sir :-)")
    sys.exit()
        