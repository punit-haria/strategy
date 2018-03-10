# This is a simple wrapper to access yahoo stock data from file.
# The user must specify a list of symbols, start date, end date, 
# and the length of window that a corresponding strategy will 
# require. The user can then call an update to the symbol windows.
# For now, we do not label each window with its corresponding 
# symbol. We assume that strategies will be symbol agnostic.
#
# Note: windows can be accessed using self.windows

import pandas
from datetime import date
import itertools

class DataAccess(object):

        def __init__(self, symbol_list, start_date, end_date, window_len=5):
                self.start = start_date
                self.end = end_date
                self.windows = []
                self.__df_array = []
                self.__start_index = 0
                self.__end_index = window_len-1
                for symbol in symbol_list:
                        # Read each symbol csv file
                        df = pandas.io.parsers.read_csv("../data/yahoo/"+symbol+".csv",\
                                        index_col=0,\
                                        parse_dates=True)
                        # Reverse order of DataFrame (old to new)
                        df = df.ix[::-1]              
                        # Take subset of DataFrame between start and end dates
                        df = df[self.start:self.end]
                        # Only require end of day price
                        df = df["Adj Close"]
                        # Append DataFrame to list
                        self.__df_array.append(df)
                        # Initialize DataFrame's corresponding window
                        self.windows.append(df.tail(window_len))

        def update_windows(self):
                for df, window in itertools.izip(self.__df_array, self.windows):
                        # Our window is a view of a subset of the dataframe (as it should be).
                        window = df.iloc[self.__start_index : self.__end_index]
                self.__start_index += 1
                self.__end_index += 1
        
# Entry point for testing---------------------------------------------

local =  DataAccess(["GOOG"], date(2012,1,1), date(2013,10,15))
for i in range(0,10):
        local.update_windows()  
        print(local.windows)
                                
