# 加载数据
library(readxl)
getwd()
df <- read_excel("data/ping2.xlsx")

# 合并数据
df
reshape2::melt(df,id.vars="na") -> df1
df1

windowsFonts(myFont = windowsFont("微软雅黑"))

# 作图
library(scales)
library(ggplot2)
df1$na<-factor(df1$na, 
                levels = df$na)
df1$variable<-factor(df1$variable,
                     levels = c('DCSS防御时时延','静态  防御时时延'))
ggplot(data=df1, aes(x=na, y=value,fill=variable)) + 
  geom_bar(stat = 'identity', position = 'dodge')+
  # geom_point(aes(x=na, y=value),pch=19,position=position_dodge(0.9),size=1)+ #绘制均值为点图
  theme_bw()+
  scale_fill_manual(values = c("#3C5488B2","#00A087B2" ))+
  theme(legend.position=c(0.05,0.95), legend.justification=c(0.05,0.95),
        legend.text=element_text(size=14,family = "myFont")) + 
  theme(legend.background = element_rect(fill = 'white', colour = 'grey')) +
  labs(fill=NULL,x="攻击者选择攻击流数量NA",y="时延(ms)",family="myFont") + 
  # scale_y_continuous(labels=df1$value) 
  scale_y_continuous(trans = log10_trans(),
                     breaks = trans_breaks("log10", function(x) 10^x),
                     labels = trans_format("log10", math_format(10^.x)))  