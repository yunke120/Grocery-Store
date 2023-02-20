import pandas as pd
import numpy as np
from xgboost import XGBRegressor
from sklearn.model_selection import train_test_split
from sklearn.metrics import mean_squared_error

class MyXgboost:
    def __init__(self) -> None:
        self.number = 0
        self.sex = ''
        self.age = 0
        self.param1 = 0
        self.param2 = 0
        self.param3 = 0
        self.investtime = ''
        self.param4 = ''
        self.stock = 0.0
        self.param5 = 0
        self.param6 = 0
        self.csv_file = './train.csv'

    def train_model(self, file):
        self.csv_file = file
        print(file)
        df=pd.read_csv(file, encoding='gb18030')
        df.replace(['No','Yes','Male','Female','< 1 Year','1-2 Year','> 2 Years'],[0,1,1,0,0,1,2],inplace=True)
        x=df.drop(labels=['股票','编号'],axis=1)
        y=df.iloc[:,-3:-2]
        xtrain,xtest,ytrain,ytest=train_test_split(x,y,test_size=0.2,random_state=420)
        axis=np.arange(2,150,10)
        r2=[]
        for i in axis:
            self.reg=XGBRegressor(n_estimators=i,
                            random_state=20).fit(xtrain,ytrain)
            r2.append(self.reg.score(xtest,ytest))

        max_d=np.arange(2,30,1)
        r2=[]
        for i in max_d:
            self.reg=XGBRegressor(n_estimators=22,
                            max_depth=i,
                            random_state=20).fit(xtrain,ytrain)
            r2.append(self.reg.score(xtest,ytest))

        lr=[0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.40]
        r2=[]
        for i in lr:
            self.reg=XGBRegressor(n_estimators=22,
                            max_depth=6,
                            learning_rate=i,
                            random_state=20).fit(xtrain,ytrain)
            r2.append(self.reg.score(xtest,ytest))

        self.reg=XGBRegressor(n_estimators=22,
                        max_depth=6,
                        learning_rate=0.3,
                        random_state=20).fit(xtrain,ytrain)
        pred=self.reg.predict(xtest)
        r2=self.reg.score(xtest,ytest)
        mse=mean_squared_error(ytest,pred)

        y1=pd.DataFrame(pred,columns=['Predict'])
        y2=pd.DataFrame(ytest)
        y2.columns=['True']
        y2.index=np.arange(y1.shape[0])
        res=pd.concat([y1,y2],axis=1)
        res.sort_values(by='True',inplace=True)

        col=xtest.columns
        feat_import=self.reg.feature_importances_
        resul=pd.DataFrame([*zip(col,feat_import)],columns=['feature_name','feature_importance'])
        resul.sort_values(by='feature_importance',axis=0,ascending=False,inplace=True)
        return 55
        
    def get_number(self, data):
        self.number = data

    def get_sex(self, data):
        self.sex = data

    def get_age(self, data):
        self.age = data
    
    def get_param1(self, data):
        self.param1 = data

    def get_param2(self, data):
        self.param2 = data

    def get_param3(self, data):
        self.param3 = data

    def get_investtime(self, data):
        self.investtime = data

    def get_param4(self, data):
        self.param4 = data

    def get_stock(self, data):
        self.stock = data

    def get_param5(self, data):
        self.param5 = data

    def get_param6(self, data):
        self.param6 = data

    def predict1(self):
        test = {
            '性别':[self.sex],
            '年龄':[self.age],
            '参数1':[self.param1],
            '参数2':[self.param2],
            '参数3':[self.param3],
            '入股时间':[self.investtime],
            '参数4':[self.param4],
            '参数5':[self.param5],
            '参数6':[self.param6],
        }
        test = pd.DataFrame(test)
        test.replace(['No','Yes','Male','Female','< 1 Year','1-2 Year','> 2 Years'],[0,1,1,0,0,1,2],inplace=True)
        return self.reg.predict(test)[0]

if __name__ == '__main__':
    app = MyXgboost()
    # 259670,Female,27,1,46,1,1-2 Year,No,19122,868,2482
    # 321964,Male,53,1,35,1,1-2 Year,Yes,18863,854,3638
    # app.train_model()
    app.get_number(321964)
    app.get_sex('Male')
    app.get_age(53)
    app.get_param1(1)
    app.get_param2(35)
    app.get_param3(1)
    app.get_investtime('1-2 Year')
    app.get_param4('Yes')
    app.get_stock(18863)
    app.get_param5(854)
    app.get_param6(3638)
    ret = app.predict1()
    print(ret)

