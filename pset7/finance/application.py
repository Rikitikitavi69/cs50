from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import gettempdir

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = gettempdir()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id = :user_id", user_id=session.get("user_id"))
    userCash = db.execute("SELECT cash FROM users WHERE id = :username", username=session.get("user_id"))[0]['cash']
    for company in portfolio:
        company["price"] = lookup(company["stock_symbol"])["price"]
        company["total"] = company["price"] * company["amount"]
    return render_template("index.html", stocks = portfolio, userCash=userCash)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    buySymbol = request.form.get("buy_symbol")
    buyShares = request.form.get("buy_shares")
    
    if request.method == "POST":
        if not buySymbol:
            return apology("\"Stock Symbol\" is empty.")
        if not buyShares :
            return apology("\"Shares\" is empty.")
        if "," in buyShares or "." in buyShares:
            return apology("You must use integer value for shares.")
        if int(buyShares) <= 0:
            return apology("Number of shares must be at lease 1!")
        if not isEnglish(buySymbol):
            return apology("Stock symbol must contain latin chars only.")
        buySymbol = buySymbol.upper()
        try:
            stock = lookup(buySymbol)
            stockName = stock["name"]
            stockPrice = stock["price"]
        except TypeError:
            return apology("Wrong stock symbol")
        userCash = db.execute("SELECT cash FROM users WHERE id = :username", username=session.get("user_id"))[0]['cash']
        total = float(buyShares) * float(stockPrice)
        
        if userCash < total:
            s = "Not enough cash." + "You have: " + usd(userCash) + ". When your total shares cost: " + usd(total) 
            return apology(s)
            
        db.execute("UPDATE users SET cash = cash - :cost WHERE id = :user_id", cost=total, user_id=session.get("user_id"))
        db.execute("INSERT INTO history (user_id, date, symbol, name, shares, price_per_share, total_price, transaction_type) \
                    VALUES (:user_id, CURRENT_TIMESTAMP, :symbol, :name, :shares, :price_per_share, :total_price, :transaction_type)",
                    user_id=session.get("user_id"), symbol=buySymbol, name=stockName, shares=buyShares, 
                    price_per_share=stockPrice, total_price=total, transaction_type="BUY")
        
        if "Error" in db.execute("SELECT stock_symbol FROM (SELECT * FROM portfolio WHERE user_id = :user_id) WHERE stock_symbol=:symbol", 
                        user_id=session.get("user_id"), symbol=buySymbol):
            db.execute("UPDATE portfolio SET amount = amount + :shares WHERE user_id = :user_id AND stock_symbol = :stock_symbol", 
                        shares=buyShares, user_id=session.get("user_id"), stock_symbol=buySymbol)
        else:
            db.execute("INSERT INTO portfolio (user_id, stock_symbol, amount, name) VALUES (:user_id, :stock_symbol, :amount, :name)", 
                        user_id=session.get("user_id"), stock_symbol=buySymbol, amount=int(buyShares), name=stockName)
        
        return redirect(url_for('index'))
        
    else:
        return render_template("buy.html")

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    
    history = db.execute("SELECT * FROM history WHERE user_id=:user_id", user_id=session.get("user_id"))
    return render_template("history.html", history=history)

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", 
                            username=request.form.get("username"))
        
        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST":
        stock = lookup(request.form.get("quote"))
        if stock == None:
            #flash("Error: wrong stock symbol!")
            return apology("Wrong stock symbol")
        #else:
            #flash(stock['symbol'] + " " + stock['name'] + " " + usd(stock['price']))

        return render_template("quoted.html", stock_symbol = stock['symbol'], stock_name = stock['name'], stock_price = usd(stock['price']))
    
    else:
        return render_template("quote.html")

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""
    
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("Missing username!")
        if not request.form.get("password"):
            return apology("Missing password!")
        if not request.form.get("password_confirm"):
            return apology("Missing password confirm!")
        if request.form.get("password") != request.form.get("password_confirm"):
            return apology("Password and re-entered password don't match.")
            
        if db.execute("SELECT username FROM users WHERE username=:username", username=request.form.get("username")) != []:
            return apology("This username is already exists.")
            
        db.execute("INSERT INTO users (username, hash) VALUES(:username, :hash)", 
                    username=request.form.get("username"), hash=pwd_context.encrypt(request.form.get("password")))
        
        return render_template("login.html")
        
    else:
        return render_template("register.html")

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    
    portfolio = db.execute("SELECT * FROM portfolio WHERE user_id=:user_id", user_id=session.get("user_id"))
    
    if request.method == "POST":
        sellSymbol = request.form.get("sellSymbol") 
        sellAmount = request.form.get("sellAmount")
        
        if (int(sellAmount) <= 0):
            return apology("Stock amount should be 1 or higher!")
        if (int(sellAmount) > int(db.execute("SELECT amount FROM portfolio WHERE user_id=:user_id AND stock_symbol=:stock_symbol", 
                                                user_id=session.get("user_id"), stock_symbol=sellSymbol)[0]["amount"])):
            return apology("You can't sell more than you have!")
        
        sellInfo = lookup(sellSymbol)

        if (int(sellAmount) == int(db.execute("SELECT amount FROM portfolio WHERE user_id=:user_id AND stock_symbol=:stock_symbol", 
                                                user_id=session.get("user_id"), stock_symbol=sellSymbol)[0]["amount"])):
            db.execute("DELETE FROM portfolio WHERE user_id=:user_id AND stock_symbol=:stock_symbol", 
                        user_id = session.get("user_id"), stock_symbol=sellSymbol)
        else:
            db.execute("UPDATE portfolio SET amount=amount-:amount WHERE user_id=:user_id AND stock_symbol=:stock_symbol", 
                        amount=sellAmount, user_id=session.get("user_id"), stock_symbol=sellSymbol)
                        
        db.execute("UPDATE users SET cash = cash + :sell WHERE id=:user_id", 
                    sell=int(sellAmount)*sellInfo['price'], user_id=session.get("user_id"))
        db.execute("INSERT INTO history (user_id, date, symbol, name, shares, price_per_share, total_price, transaction_type) \
                    VALUES (:user_id, CURRENT_TIMESTAMP, :symbol, :name, :shares, :price_per_share, :total_price, :transaction_type)",
                    user_id=session.get("user_id"), symbol=sellSymbol, name=sellInfo["name"], shares=sellAmount, 
                    price_per_share=sellInfo["price"], total_price=int(sellAmount)*sellInfo["price"], transaction_type="SELL")
            
        return redirect(url_for("index"))
        
    else:
        return render_template("sell.html", stocks=portfolio)
        
@app.route("/change_password", methods=["GET","POST"])
@login_required
def changePassword():
    """Changes user's password."""
    
    if request.method == "POST":
        oldPwd = db.execute("SELECT hash FROM users WHERE id=:user_id", user_id=session.get("user_id"))[0]["hash"]
        newPwdOld = request.form.get("oldPassword")
        newPwd = request.form.get("newPassword")
        newPwdConfirm = request.form.get("newPasswordConfirm")

        if (newPwdOld == "" or newPwd == "" or newPwdConfirm == ""):
            return apology("One or more fields are empty!")
        if (newPwd != newPwdConfirm):
            return apology("New password doesn't match confirmation!")
        print(session.get("user_id"))
        if not (pwd_context.verify(newPwdOld, oldPwd)):
            return apology("The old password is incorrect!")
            
        db.execute("UPDATE users SET hash = :hash WHERE id=:user_id", 
                    hash=pwd_context.encrypt(newPwd), user_id=session.get("user_id"))
        return redirect(url_for("logout"))
        
    else:
        return render_template("change_password.html")