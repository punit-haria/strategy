import pandas
from os import listdir
from os.path import isfile, join

class StockData(object):
    """Stock/equity object"""

    def __init__(self,symbol,dframe):
        self.symbol = symbol
        self.df = dframe
     

"""Parses data based on Yahoo stock data format"""
def YahooStock( filepath ):
    #read data
    heads = ['DATE','OPEN','HIGH','LOW','CLOSE','VOLUME','ADJ_CLOSE']
    df = pandas.read_csv(filepath,skipinitialspace=True,\
        header=0,names=heads,parse_dates=['DATE'],index_col='DATE' )
    if not df.index.is_monotonic:
        df.sort_index(inplace=True)
    return df

"""Parses data based on Investopedia stock data format"""
def InvestopediaStock( filepath ):
    #read data
    heads = ['DATE','OPEN','HIGH','LOW','CLOSE','VOLUME']
    df = pandas.read_csv(filepath,skipinitialspace=True,\
        header=0,names=heads,parse_dates=['DATE'],index_col='DATE' )
    if not df.index.is_monotonic:
        df.sort_index(inplace=True)
    return df

"""Parses data based on Quantquote stock data format"""
def QuantquoteStock( filepath ):
    #read data
    heads = ['DATE','TIME','OPEN','HIGH','LOW','CLOSE','VOLUME']
    df = pandas.read_csv(filepath,skipinitialspace=True,\
        header=0,names=heads,parse_dates=['DATE'],index_col='DATE' )
    if not df.index.is_monotonic:
        df.sort_index(inplace=True)
    return df

"""Converts all csv files in a folder to stocks"""
def getFolderData( folderpath, strategy=None ):
    print "Getting data from '%s'" % folderpath
    #Get list of files in directory
    onlyfiles = [ f for f in listdir(folderpath) if\
        isfile(join(folderpath,f)) ]
    
    #Create stockData object representing each file
    array = [] 
    for file in onlyfiles:
        symbol = file.replace(".csv","")
        df = strategy( folderpath+"/"+file )
        stockdat = stk.StockData(symbol,df)
        array.append(stockdat)
    return array

