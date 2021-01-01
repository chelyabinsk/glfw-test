setwd("/home/pan/Documents/glfw-test")
d <- read.csv("fft_out.csv")
rate <- 44100
M <- 73728
step <- rate/M
d[,1] <- 2*d[,1]/M*rate
plot(d[,1],d[,2],type='l')
