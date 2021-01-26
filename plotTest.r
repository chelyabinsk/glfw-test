setwd("/home/pan/Documents/glfw-test")
#d <- read.csv("fft_python.csv")
d <- read.csv("fft_out.csv")
rate <- 48000
M <- 73728
step <- rate/M
d[,1] <- d[,1]/M*rate

num_groups <- 100

# Groupings
groups <- rep(0,num_groups)
step_size <- 1/num_groups
y0 <- 15
x0 <- 0
y1 <- M/2
x1 <- 1
b <- log(y0/y1)/(x0-x1)
a <- y0*exp(-b*x0)
c <- 1
for(j in seq(x0,x1,length.out = num_groups)){
  groups[c] <- a*exp(j*b)
  c <- c + 1
}

# groups <- rep(0,num_groups)
# y0 <- 15
# x0 <- 0
# y1 <- M/2
# x1 <- 1
# b <- log(y1/y0)/log(x1/x0)
# a <- y0*x0^(-b)
# c <- 1
# for(j in seq(x0,x1,length.out = num_groups)){
#   groups[c] <- a*(j^b)
#   c <- c + 1
# }


groups
#plot(d[,1],ifelse(d[,2]>quantile(d[,2],0.999),d[,2],0),type='l')
plot(d[,1],d[,2],type='l')
for(i in groups){
  lines(rep(i/M*rate,2),seq(0,2^32,length.out = 2),col="red")
}

grouped <- rep(0,num_groups)

group_pos = 1
qqq <- as.vector(quantile(d[,2],0.995))#max(d[,2])*0.75#0#as.vector(quantile(d[,2],0.9995))
for(i in 1:(M/2-1)){
  if(groups[group_pos]<=(i+1)*step){
    group_pos <- group_pos + 1
  }
  if(group_pos>=num_groups){
    group_pos = num_groups
  }
  if(d[i,2] > qqq){
    grouped[group_pos] <- grouped[group_pos] + d[i,2]
  }
  i
}

barplot(grouped/(2^31))
groups
