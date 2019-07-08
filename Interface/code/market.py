from cmd import Cmd

import time
import binance
import json
import unicodedata
import os
from pathlib import Path
from binance.client import Client
from binance.enums import *

import main

class Market():
	#l = 0
	#m = 0
	#b = 0
	#s = 0

	def xch(self, xtype1, xtype2, xsymbol, xquantity, xprice, api_key ):
		client = Client(api_key, CONST_SECRET_KEY)
		print("Exchange Interface Initialised")
		time.sleep(1)
		print("Formulating the order...")
		time.sleep(1)

		#Time to sort the parameters

		#xtype1
		if xtype1.lower() == ("buy"):

			if xtype2.lower() == ("market"):

				print("Market buying " + str(xquantity) + " " +  xsymbol + " at the price of " + str(xprice) + ".")

				order = client.order_market_buy(
					symbol=xsymbol,
					quantity=xquantity)
					#price = xprice

				print("Order completed.")


			elif xtype2.lower() == ("limit"):

				print("Limit buying " + str(xquantity) + " " +  xsymbol + " at the price of " + str(xprice) + ".")

				order = client.order_limit_buy(
					symbol=xsymbol,
					quantity=xquantity)
					#price = xprice

				print("Order completed.")

			else:
				print("ERROR: Unknown input")
				return

		elif xtype1.lower() == ("sell"):

			if xtype2.lower() == ("market"):

				print("Market selling " + str(xquantity) + " " +  xsymbol + " at the price of " + str(xprice) + ".")

				order = client.order_market_sell(
					symbol=xsymbol,
					quantity=xquantity)
					#price = xprice

				print("Order completed.")

			elif xtype2.lower() == ("limit"):

				print("Limit selling " + str(xquantity) + " " +  xsymbol + " at the price of " + str(xprice) + ".")

				order = client.order_limit_sell(
					symbol=xsymbol,
					quantity=xquantity)
					#price = xprice

				print("Order completed.")

			else:
				print("ERROR: Unknown input")
				return

		else:
			print("ERROR: Unknown input")
			return

		#EOF



