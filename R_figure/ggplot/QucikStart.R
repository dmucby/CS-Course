library(gapminder)
library(ggplot2)
head(gapminder, 20)

p <- ggplot(data = gapminder, 
            mapping = aes(
              x = gdpPercap,
              y =  lifeExp))

# 可以简写成 p <- ggplot(gapminder, aes(gdpPercap, lifeExp))

# 作图步骤通过+连接
p + geom_point()

p + geom_smooth() + geom_point(method="lm") # 拟合曲线

# 改善坐标范围
p + geom_point() +
  geom_smooth(method="gam") +
  scale_x_log10()

# 改善刻度
p + geom_point() +
  geom_smooth(method="gam") +
  scale_x_log10(labels=scales::dollar)

# 颜色、符号、线性等映射
# 用不同颜色代表不同大洲，将continent 变量映射到color
p <- ggplot(data=gapminder,
            mapping = aes(
              x = gdpPercap,
              y = lifeExp,
              color = continent))

p + geom_point() +
  geom_smooth(method="loess") +
  scale_x_log10(labels=scales::dollar)


# 条形图
library(socviz)
p <- ggplot(data = gss_sm,
            mapping = aes(x = bigregion))
p + geom_bar()

library(readxl)
data_excel <- read_excel("G:\\D\\微信文件\\WeChat Files\\wxid_lzg5pq4gir5j22\\FileStorage\\File\\2022-08\\数据记录.xlsx")
head(data_excel, 20)
p <- ggplot(data = data_excel,
            mapping = aes(
              x = 防御n,
              y = `PING时间(ms)`))
p
p + geom_bar()


