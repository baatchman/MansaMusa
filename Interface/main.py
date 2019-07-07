
#Importing modules
import sys
from cmd import Cmd
import binance
import json
import unicodedata
import os
from pathlib import Path
from binance.client import Client
from binance.enums import *
import argparse

#Importing scripts
sys.path.insert(0, 'code/')
import market

class Prompt(Cmd):

	def do_hello(self, args):
		"""Says hello. If you provide a name, it will greet you with it."""
		if len(args) == 0:
			name = 'stranger'
		else:
			name = args
			print("Hello, %s" % name)

	def do_quit(self, args):
		"""Quits the program."""
		print ("Quitting.")
		raise SystemExit

	def do_addAccount(self, args):
		"""Adds a new account."""
		m = Main()
		m.setupAccounts()

	def do_order(self, *args):
		"""Places an order.\n
		Arguments in order:\n
		Buy or Sell option.\n
		Market or Limit option.\n
		The crypto symbol.\n
		The quantity of the order.\n
		The price of the order.\n"""
		
		args = args
		m = market.Market()
		m.xch(*args)


class Main():

	def addAccount(self):
		a_keyamount = input("How many accounts would you like to add, sir?")
		a_name = ""
		a_key = ""

		f=open("data/accounts.txt", "a")
		for i in range(a_keyamount):
			a_key = raw_input("What is the API Key for this account, sir? > ")
			a_name = raw_input("What would you like to call this account, sir? > ")
			f.write(name + "\n" + str(key) + "\n")


		
	def setupAccounts(self):
		print("It seems as though you don't have your accounts set up, I'll get to doing that right now. I just need to ask you some questions :-)")
		keyamount = input("How many accounts do you have sir? > ")
		key = ""
		name = ""
		
		f=open("data/accounts.txt","w+")
		for i in range(keyamount):
			key = raw_input("What is the API Key for the account, sir? > ")
			name = raw_input("What would you like to call this account, sir? > ")
			f.write(name + "\n" + str(key) + "\n")
		f.close()
		print("Thank you sir, now we can set up the program.")

	def initAccounts(self):
		print("Loading Accounts...")
		accfile = Path("data/accounts.txt")
		if accfile.is_file():
			if os.path.getsize("data/accounts.txt") == 0:
				print("No accounts in the account file, let's fix this shall we?")
				os.remove("data/accounts.txt")
				self.setupAccounts()
			prompt = Prompt()
			prompt.prompt = '> '
			prompt.cmdloop('CRYPTOANAYLISIS: PREDICTOR')
		else:
			self.setupAccounts()


if __name__ == '__main__':
	m = Main()
	m.initAccounts()
