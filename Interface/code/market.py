from cmd import Cmd

import binance
import json
import unicodedata
import os
from pathlib import Path
from binance.client import Client
from binance.enums import *
#client = Cloooient('J2YbY1UFBWXnHWPtLCuzaG2oxoEYq1ESoYVs1rAbr3bQbR2bzFoTSlqLn5xpMYR7', 'CfmFxa9QqBljaXHUIdpv0fDZchzQCo5O0JroVX0Ws1fovsgahTw2OlbIVemjfpT0')


import main

class Market():
	#l = 0
	#m = 0
	#b = 0
	#s = 0

	def xch(self, xtype1, xtype2, xsymbol, xquantity, xprice ):

		print("Formulating the order...")

		#Time to sort the parameters

		#xtype1
		if strtolower(xtype1) == ("buy"):

			if strtolower(xtype2) == ("market"):

				print("Market buying " + xquantity + " " +  xsymbol + " at the price of " + xprice + ".")

				order = client.order_market_buy(
					symbol=xsymbol,
					quantity=xquantity,
					price=xprice)

				print("Order completed.")


			elif strtolower(xtype2) == ("limit"):

				print("Limit buying " + xquantity + " " +  xsymbol + " at the price of " + xprice + ".")

				order = client.order_limit_buy(
					symbol=xsymbol,
					quantity=xquantity,
					price=xprice)

				print("Order completed.")

			else:
				print("ERROR: Unknown input")
				return

		elif strtolower(xtype1) == ("sell"):

			if strtolower(xtype2) == ("market"):

				print("Market selling " + xquantity + " " +  xsymbol + " at the price of " + xprice + ".")

				order = client.order_market_sell(
					symbol=xsymbol,
					quantity=xquantity,
					price=xprice)

				print("Order completed.")

			elif strtolower(xtype2) == ("limit"):

				print("Limit selling " + xquantity + " " +  xsymbol + " at the price of " + xprice + ".")

				order = client.order_limit_sell(
					symbol=xsymbol,
					quantity=xquantity,
					price=xprice)

				print("Order completed.")

			else:
				print("ERROR: Unknown input")
				return

		else:
			print("ERROR: Unknown input")
			return

		#EOF



