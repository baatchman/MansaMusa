#!/usr/bin/python

#Import modules
import click
from pathlib import Path
import os
import sys

#Import scripts
sys.path.insert(0, 'code/')
import market

#Setup command line arguments
@click.command()
@click.option('-a', type=click.STRING, help='Indicates where your account file is.')
@click.option('-t', type=click.Choice(['buy','sell']), help='Your choice of transaction')
@click.option('-o', type=click.Choice(['market','limit']), help='Your choice of order')
@click.option('-q', type=click.INT, help='How much of the currency you would like to buy/sell')
@click.option('-p', type=click.INT, help='What price you will bargain for.')
@click.option('-c', type=click.STRING, help='The crypto token of your choice.')

#Function
def init(a, t, o, q, p, c):
    #Load the account file
    click.echo("Loading accounts...")
    accfile = Path(a)
    if accfile.is_file():
        if os.path.getsize(a) == 0:
            click.echo("ERROR: No accounts in the account file, please put them in.")
            sys.exit()
        click.echo("Accounts loaded")
        #Transition to the market interface.
        click.echo("Setting up exchange interface")
        m = market.Market()
        m.xch(t, o, c, q, p)
    else:
        click.echo("ERROR: The specified file does not exist.")
        sys.exit()

    

if __name__ == '__main__':
    init()
