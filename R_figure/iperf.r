# 加载数据
library(readxl)
getwd()
df <- read_excel("data/iperf.xlsx")

# 合并数据
df
reshape2::melt(df,id.vars="n^a") -> df1
df1

# 作图
library(scales)
library(ggplot2)
df1$`n^a`<-factor(df1$`n^a`, 
               levels = df$`n^a`)
df1$variable<-factor(df1$variable,
                     levels = c('DCSS防御时带宽','静态  防御时带宽'))
ggplot() + 
  geom_line(data=df1, aes(x=`n^a`, y=value,group=variable,color=variable),size =2)+
  # geom_point(aes(x=`n^a`, y=value),pch=19,position=position_dodge(0.9),size=1)+ #绘制均值为点图
  theme_bw()+
  scale_color_manual(values = c("#2166AC","#B2182B"))+
  theme(legend.position=c(0.95,0.9), legend.justification=c(0.9,0.9),
        legend.title=element_blank(),legend.text=element_text(size=14,family = "myFont")) + 
  theme(legend.background = element_rect(fill = 'white', colour = 'grey')) +
  labs(fill=NULL,x=paste("攻击者选择攻击流数量NA"),y="带宽(Kbit/s)")
  # scale_y_continuous(labels=df1$value) 
