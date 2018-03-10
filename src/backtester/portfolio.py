# Portfolio manager that takes three inputs into consideration: 
# the transaction model, risk model, and a blend of alpha models.
# Based on the three advisors, the porfolio determines the current
# best positions. The difference between the previous holdings and
# the newly determined holdings tell us what trades need to be made.

import time
import datetime
import numpy as np

class Portfolio(object):
    
    # Portfolio constructor requires list of all securities available
    # for trade, plus the start and end dates of the backtest
    def __init__(self, symbol_list, start_date, end_date):
        # List of column labels for readability
        self.holdings_labels = ['cash']
        self.holdings_labels.extend(symbol_list)
        self.holdings_labels = np.array(self.holdings_labels)
        # Holdings is a list of numpy.arrays of asset proportions
        self.holdings = []
        # Initially we have only cash
        self.current = np.zeros(len(self.holdings_labels))
        self.current[0] = 1.0  
        # Initialize previous portfolio asset allocations           
        self.previous = self.current
        # Append initial portfolio to holdings
        self.holdings.append(self.current)
        # Keep a record of start and end dates
        self.start = start_date
        self.end = end_date
    
    # Generates portfolio holdings between start and end dates
    # using provided transaction model, risk model, and blend of
    # alpha models
    def generate_holdings(self):
        pass        
   
    # Save portfolio history to disk
    def save_holdings(self):
        timestamp = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H.%M.%S')
        f_handle = file('holdings_log/'+timestamp+'.csv','a')
        np.savetxt(f_handle, self.holdings_labels[None], fmt='%s', delimiter=',')
        for row in self.holdings:
            np.savetxt(f_handle, row[None], fmt='%1.3f',delimiter=',')
        
        
# Entry point for testing:
pf = Portfolio(['AAPL','GOOG','XOM'],\
    datetime.date(2012,1,1),\
    datetime.date(2013,1,1))
print(pf.holdings_labels)        
print(pf.current)
