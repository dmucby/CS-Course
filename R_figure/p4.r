# 加载数据
library(readxl)
getwd()
df <- read_excel("data/ping1.xlsx")

# 合并数据
df
reshape2::melt(df,id.vars="防御n") -> df1
df1

# 设置格式
fancy_scientific <- function(l) {
  # turn in to character string in scientific notation
  l <- format(l, scientific = TRUE)
  l <- gsub("0e\\+00","0",l)
  # quote the part before the exponent to keep all the digits
  l <- gsub("^(.*)e", "'\\1'e", l)
  # remove "+" after exponent, if exists. E.g.: (3x10^+2 -> 3x10^2) 
  l <- gsub("e\\+","e",l)
  # turn the 'e+' into plotmath format
  l <- gsub("e", "%*%10^", l)
  # convert 1x10^ or 1.000x10^ -> 10^ 
  l <- gsub("\\'1[\\.0]*\\'\\%\\*\\%", "", l)
  # return this as an expression
  parse(text=l)
}

# 作图
library(scales)
library(ggplot2)
df1$防御n<-factor(df1$防御n, 
                  levels = df$防御n)
ggplot(data=df1, aes(x=防御n, y=value,fill=防御n)) + 
  geom_bar(stat = 'identity', position = 'dodge')+
  # geom_point(aes(x=防御n, y=value),pch=19,position=position_dodge(0.9),size=1)+ #绘制均值为点图
  theme_bw()+
  scale_fill_manual(values = c("#B2182B","#D6604D","#F4A582","#FDDBC7",
                               "#D1E5F0","#92C5DE","#4393C3","#2166AC"))+
  theme(legend.position=c(0.9,0.9), legend.justification=c(0.9,0.9)) + 
  theme(legend.background = element_rect(fill = 'white', colour = 'grey')) +
  labs(fill=NULL,x="防御n",y="Latency(ms)") + 
  # scale_y_continuous(labels=df1$value) 
  scale_y_continuous(trans = log10_trans(),
                     breaks = trans_breaks("log10", function(x) 10^x),
                     labels = trans_format("log10", math_format(10^.x)))  
  # geom_errorbar(aes(ymin = value-ci, ymax=value+ci), #误差条表示95%的置信区间
  #              width=0.1, #误差条末端短横线的宽度
  #              position=position_dodge(0.9), 
  #              color="black",
  #              alpha = 0.7,
  #              size=0.5) 