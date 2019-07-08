#!/usr/bin/python

#Import modules
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

#Setup command line arguments
@click.command()
@click.option('-a', type=click.STRING, help='Indicates where your account file is.')
@click.option('-t', type=click.Choice(['buy','sell']), help='Your choice of transaction')
@click.option('-o', type=click.Choice(['market','limit']), help='Your choice of order')
@click.option('-q', type=click.FLOAT, help='How much of the currency you would like to buy/sell')
@click.option('-p', type=click.FLOAT, help='What price you will bargain for.')
@click.option('-c', type=click.STRING, help='The crypto token of your choice.')

#Function
def init(a, t, o, q, p, c):
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
        #Load the account key
        with open(accfile, "r") as ins:
            array = []
            for line in ins:
                array.append(line)
        click.echo("Account loaded.")
        time.sleep(1)       
        #Transition to the market interface.
        click.echo("Setting up exchange interface")
        time.sleep(1)
        client = Client(array[0], CONST_SECRET_KEY)
        m = market.Market()
        m.xch(t, o, c, q, p, array[0])
    else:
        click.echo("ERROR: The specified file does not exist.")
        sys.exit()

    

if __name__ == '__main__':
    init()
