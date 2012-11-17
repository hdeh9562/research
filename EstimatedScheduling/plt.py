import matplotlib.pyplot as plt
fh = open( "result_FFD.txt" );
x = []
number_pareto_point=0;
for line in fh.readlines():
    y = [float(value) for value in line.split()]
    number_pareto_point = len(y)
    x.append( y )
fh.close()
print x
p1=plt.plot(x[0], x[1], 'ro') # 'r--'  'bs' 'g^'

print 'number_pareto_point',number_pareto_point
fh = open( "result_BLIND_GREEDY.txt" );
xgreedy = []
for line in fh.readlines():
    ygreedy = [float(value) for value in line.split()]
    xgreedy.append( ygreedy )
fh.close()

fhgeneral = open( "GENERAL_INFORMATION.txt" );
general_information = []
for line in fhgeneral.readlines():
    general_information_item = [float(value) for value in line.split()]
    general_information.append(general_information_item)
fhgeneral.close()
print 'general_information'
print general_information 

minx= float( min(x[0])) 
minx= min(minx,min(xgreedy[0])) -.1*min(x[0])

maxx= float(max(x[0])) 
maxx= max(maxx,max(xgreedy[0])) +.1*max(x[0])

miny= float( min(x[1]))
miny= min(miny,min(xgreedy[1]))  - .1*min(x[1])
maxy= float(max(x[1])) 
maxy= max(maxy,max(xgreedy[1])) + .1*max(x[1])
 
plt.axis([minx,maxx , miny, maxy])
#plt.text(4, 15, r'$\mu=100,\ \sigma=15$')
plt.xlabel('Make-span (Hour)', fontsize=18, color='blue')
plt.ylabel('Total Cost ($)', fontsize=18, color='blue')
#plt.title('histogram of x')
plt.title('Achieved time and cost, n='+ str(int(general_information[0][0])), fontsize=18, color='blue') #r'$\sigma_i=15$ ' )
print xgreedy[0][0],xgreedy[1][0]
strinttoshowongreedypoint = '(' + str(round(float(xgreedy[0][0]), 1)) + 'H, $'  + str(round(float(xgreedy[1][0]),2)) + ')'
#greedy annotate
plt.annotate(strinttoshowongreedypoint , xy=(xgreedy[0][0], xgreedy[1][0]), xytext=(xgreedy[0][0]-.15*xgreedy[0][0], xgreedy[1][0]-.03*xgreedy[1][0]),
           arrowprops=dict(arrowstyle="->",
                                connectionstyle="arc,angleA=0,armA=30,rad=10"),
            )
#our best pareto point annotate
strinttoshowon_best_makespan_point= '(' + str(round(float(x[0][0]), 1)) + 'H, $'  + str(round(float(x[1][0]),2)) + ')'
plt.annotate(strinttoshowon_best_makespan_point , xy=(x[0][0], x[1][0]), xytext=(x[0][0]-.08*x[0][0], x[1][0]+.01*x[1][0]),
           arrowprops=dict(arrowstyle="->",
                                connectionstyle="arc,angleA=0,armA=30,rad=10"),
            )
#our best cost pareto point annotate
strinttoshowon_best_cost_point= '(' + str(round(float(x[0][number_pareto_point-1]), 1)) + 'H, $'  + str(round(float(x[1][number_pareto_point-1]),2)) + ')'
plt.annotate(strinttoshowon_best_cost_point , xy=(x[0][number_pareto_point-1], x[1][number_pareto_point-1]), xytext=(x[0][number_pareto_point-1]-.08*x[0][number_pareto_point-1], x[1][number_pareto_point-1]-.04*x[1][number_pareto_point-1]),
           arrowprops=dict(arrowstyle="->",
                                connectionstyle="arc,angleA=0,armA=30,rad=10"),
            )




p2 = plt.plot(xgreedy[0], xgreedy[1], 'bs')

plt.legend( (p1[0], p2[0]), ('Proposed Algorithm', 'Greeedy Algorithm') )
 
plt.grid(True)
string_file_name = './images/result_plot_n' + str(int(general_information[0][0])) + '_m' + str(int(general_information[1][0]))+ '_H' + str(int(general_information[2][0])) +'_min' + str(int(general_information[5][0])) +'_max' + str(int(general_information[6][0]))+ '.eps'
string_file_name_jpg = './images/result_plot_n' + str(int(general_information[0][0])) + '_m' + str(int(general_information[1][0]))+ '_H' + str(int(general_information[2][0])) +'_min' + str(int(general_information[5][0])) +'_max' + str(int(general_information[6][0]))+ '.jpg'
plt.savefig(string_file_name)
plt.savefig(string_file_name_jpg)
plt.show()
