d.cancer <- readr::read_csv("data/cancer.csv", 
                            locale=locale(encoding="GBK"))
res1 <- table(d.cancer[,'sex']); 
print(res1)
barplot(res1)
barplot(res1, main="性别分布", 
        col=c("brown2", "aquamarine1"))
head(colors(), 6)

sqrt(exp(1.0))

exp(1.0) |> log()

frat <- function(x){
  (1 + x + 2*x^2)/(1 + 3*x + 2*x^2 + x^3)
}

# 打印结果
print(sin(pi/2))

# 正常输出
cat("sin(pi/2)=", sin(pi/2), "\n")

# 记录文本型输出结果
sink("tmpres01.txt", split = TRUE)
print(sin(pi/6))
print(cos(pi/6))
cat("t(10)的双侧0.05分位数（临界值）=", qt(1 - 0.05/2, 10), "\n")
sink()

# 赋值
x1 <- 1:10
x1

# 声明向量
marks <- c(3,5,10,5,6)
marks

# 绘图示例
curve(x^2, -2, 2)

curve(sin(x), 0, 2*pi)
abline(h=0) # abline添加参考线

# 条形图例
barplot(c("男生"=10, "女生"=7), 
        main="男女生人数")

demo("graphics")
demo("image")






