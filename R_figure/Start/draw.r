tax.tab <- read.csv("taxsamp.csv", header = TRUE, as.is = TRUE)
print(head(tax.tab))
# header=TRUE指明第一行作为变量名行， 
# 选项as.is=TRUE说明字符型列要原样读入而不是转换为因子(factor)

d.cancer <- readr::read_csv("data/cancer.csv", 
                            locale=locale(encoding="GBK"))

x <- rnorm(30, mean=100, sd=1)
print(round(x,2))
# 直方图
hist(x)
# col 指定颜色
hist(x, col=rainbow(15), 
     main='正态随机数', xlab='', ylab='频数')

tmp.dens <- density(x)
hist(x, freq=FALSE,
     ylim=c(0,max(tmp.dens$y)),
     col=rainbow(15),
     main='正态随机数',
     xlab='', ylab='频数')
lines(tmp.dens, lwd=2, col='blue')

# 多条曲线图
x <- seq(0, 2*pi, length=200)
y1 <- sin(x)
y2 <- cos(x)
matplot(x, cbind(y1, y2), type='l',
        lty=1,lwd=2,col = c("red", "blue"),
        xlab = "x", ylab="y1,y2")
abline(h=0, col='gray')

# lines 增加曲线
# legend 增加标注
x <- seq(0, 2*pi, length=200)
y1 <- sin(x)
y2 <- cos(x)
plot(x, y1, type='l', lwd=2, col="red")
lines(x, y2, lwd=2, col="blue")
abline(h=0, col='gray')
legend(0, -0.5, col=c("red", "blue"),
       lty=c(1,1), lwd=c(2,2), 
       legend=c("sin", "cos"))

library(gapminder)
head(gapminder, 20)
library(socviz)
head(gss_sm, 20)









