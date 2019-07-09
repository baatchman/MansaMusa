from cmd import Cmd

import datetime
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

	def xch(self, xtype1, xtype2, xsymbol, xquantity, xprice, p_key, s_key):
		client = Client(p_key, s_key)
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
                                f = open("../debug/log.txt", "a+")
                                f.write("SUCCEES: Market Bought " + xquantity + " " + xsymbol + " at " + datetime.datetime.now())
                                f.close()
                                return

			elif xtype2.lower() == ("limit"):

				print("Limit buying " + str(xquantity) + " " +  xsymbol + " at the price of " + str(xprice) + ".")

				order = client.order_limit_buy(
					symbol=xsymbol,
					quantity=xquantity)
					#price = xprice

				print("Order completed.")
                                f = open("../debug/log.txt", "a+")                                                                  f.write("SUCCEES: Limit Bought " + xquantity + " " + xsymbol + " at " + datetime.datetime.now())                                                     f.close()                                         return
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

                                f = open("../debug/log.txt", "a+")                                                                  f.write("SUCCEES: Market Sold " + xquantity + " " + xsymbol + " at " + datetime.datetime.now())                                                     f.close()                                         return

			elif xtype2.lower() == ("limit"):

				print("Limit selling " + str(xquantity) + " " +  xsymbol + " at the price of " + str(xprice) + ".")

				order = client.order_limit_sell(
					symbol=xsymbol,
					quantity=xquantity)
					#price = xprice

				print("Order completed.")

                                f = open("../debug/log.txt", "a+")                                                                  f.write("SUCCEES: Limit sold " + xquantity + " " + xsymbol + " at " + datetime.datetime.now())                                                     f.close()                                         return

			else:
				print("ERROR: Unknown input")
				return

		else:
			print("ERROR: Unknown input")
			return

		#EOF



