#!/usr/bin/python

#Import modules
from array import *
import time
import click
from pathlib import Path
import os
import sys
import binance
from binance.client import Client
from binance.enums import *

#Import scripts
sys.path.insert(0, 'code/')
import market
import setupaccounts

#Setup command line arguments
@click.command()
@click.option('-a', type=click.STRING, help='Indicates where your account file is.')
@click.option('-t', type=click.Choice(['buy','sell']), help='Your choice of transaction')
@click.option('-o', type=click.Choice(['market','limit']), help='Your choice of order')
@click.option('-q', type=click.FLOAT, help='How much of the currency you would like to buy/sell')
@click.option('-p', type=click.FLOAT, help='What price you will bargain for.')
@click.option('-c', type=click.STRING, help='The crypto token of your choice.')
@click.option('--setup', type=click.BOOL, default='false', help="Sends you to a user friendly program to set your account file, use '--setup true' to do this.")

#Main function
def init(a, t, o, q, p, c, setup):
    if setup == True:
        setupaccounts.main()
    else:
        #Load the accounts
        click.echo("Loading accounts...")
        time.sleep(1)
        accfile = Path(a)
        #Find the account file
        click.echo("Detecting Account file...")
        time.sleep(1)
        if accfile.is_file():
            if os.path.getsize(a) == 0:
                click.echo("ERROR: No accounts in the account file, please put them in.")
                sys.exit()
            click.echo("Account file detected.")
            time.sleep(2)
            #Find the number of lines in file
            num_lines = 0
            with open(accfile, "r") as f:
                for line in f:
                    num_lines += 1
            #Put account keys into 2d array
            duo = 0
            keyarray = []
            bruh = open(accfile, "r")
            while duo != num_lines / 2:
                p_key = bruh.readline()
                s_key = bruh.readline()
                p_key_clean = p_key.rstrip("\n\r")
                s_key_clean = s_key.rstrip("\n\r")
                keyarray.insert(duo, [p_key_clean, s_key_clean])
                duo += 1
            click.echo("Accounts loaded.")
            time.sleep(1)       
            #Transition to the market interface.
            click.echo("Setting up exchange interface")
            time.sleep(1)
            traderLoop(t, o, q, p, c, keyarray)
        else:
            click.echo("ERROR: The specified file does not exist.")
            sys.exit()

def traderLoop(xT, xO, xQ, xP, xC, ar):
    for row in ar:
        client = Client(row[0], row[1])
        info = client.get_account()
        if not info['balances']:
            print("ERROR: No balance in this account, moving on to the next one")
        else:
            m = market.Market()
            m.xch(xT, xO, xC, xQ, xP, row[0], row[1])
            return
    #client = Client(ar[0][0], ar[0][1])
    #status = client.get_account()
    #print(status)
    #m = market.Market()
    #m.xch(xT, xO, xC, xQ, xP, ar[0][0], ar[0][1])

if __name__ == '__main__':
    init()
