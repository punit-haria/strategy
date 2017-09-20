import MySQLdb
from script import csvparser as csv

#connect to database
db = MySQLdb.connect("localhost","root","newpass")

#prepare database and tables
cursor = db.cursor()
cursor.execute("CREATE DATABASE IF NOT EXISTS eodprices")
cursor.execute("USE eodprices")
cursor.execute("DROP TABLE IF EXISTS yahoostocks")
sql = ("CREATE TABLE yahoostocks ("
    "symbol CHAR(8) NOT NULL,"
    "eod_date DATE NOT NULL,"
    "open DECIMAL(20,2),"
    "high DECIMAL(20,2),"
    "low DECIMAL(20,2),"
    "close DECIMAL(20,2),"
    "volume DECIMAL(24,2),"
    "adj_close DECIMAL(20,2),"
    "PRIMARY KEY (symbol, eod_date)"
    ")")
cursor.execute(sql)

#get eod stock data from yahoo
yahoo = csv.getFolderData("data/yahoo",csv.YahooStock)

#insert data into table
for stock in yahoo:
    symbol = stock.symbol
    df = stock.df
    print "Adding %s..." %symbol
    for index, row in df.iterrows():
        value = "'"+symbol+"','"+index.strftime("%Y-%m-%d")+"',"
        value += str(row['OPEN'])+","+str(row['HIGH'])+","
        value += str(row['LOW'])+","+str(row['CLOSE'])+","
        value += str(row['VOLUME'])+","+str(row['ADJ_CLOSE'])
        sql = ("INSERT INTO yahoostocks("
           "symbol, eod_date, open, high, low,"
           "close, volume, adj_close)"
           "VALUES ("+value+")")
        try:
            cursor.execute(sql)
            db.commit()
        except:#rollback incase of error
            db.rollback()

#close connection
db.close()