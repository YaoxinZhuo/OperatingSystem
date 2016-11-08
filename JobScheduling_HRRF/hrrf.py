# coding=utf-8

#用字典替代结构体
node = { 'number':'',      #作业号
        'reach_time':'',   #作业抵达时间
        'need_time':'',    #作业的执行时间
        'privilege':'',    #作业优先权
        'excellent':'',    #响应比
        'start_time':'',   #作业开始时间
        'wait_time':'',    #等待时间
        'tr_time':'',      #周转时间
        'wtr_time':'',     #带权周转时间
        'run_time':'',     #作业累计执行时间
        'visited':'',      #作业是否被访问过
        'isreached':''}    #作业是否已抵达

#列表存储字典
list = []

#定义全局变量，用来计算平均等待时间、平均周转时间、平均带权周转时间
dic_time = {'all_wait_time':'',
            'all_tr_time':'',
            'all_wtr_time':'',
            'all_jobs':''
            }

#初始化所有作业信息
def initial_jobs():
    for key in node:
        node[key] = 0
    for key in dic_time:
        dic_time[key] = 0

#读取文件数据
def read_jobdata():
    #打开并读取所有数据
    file = open('/Users/TT/Desktop/Jobdata.txt').read()
    data_list = file.split()
    i = 0
    while i<len(data_list):
        dic = node.copy()
        #写入数据
        dic['number'] = int(data_list[i])
        dic['reach_time'] = float(data_list[i+1])
        dic['need_time'] = float(data_list[i+2])
        dic['privilege'] = float(data_list[i+3])
        list.append(dic)
        i = i + 4
    dic_time['all_jobs'] = len(list)

# 任务完成后移出队列
def remove_jobs(number):
    for i in range(0,len(list)):
        if list[i]['number'] == number:
            tag = i
    list.pop(tag)

#输出
def output(number):
    for i in range(0,len(list)):
        if list[i]['number'] == number:
            tag = i
    print '执行完的作业是:%d号作业,等待时间为:%d,周转时间为:%d,带权周转时间为:%.2f' %(list[tag]['number'],list[tag]['wait_time'],list[tag]['tr_time'],list[tag]['wtr_time'])
    if len(list)==1:
        print '平均等待时间:%.2f' %(dic_time['all_wait_time']/dic_time['all_jobs'])
        print '平均周转时间:%.2f' % (dic_time['all_tr_time'] / dic_time['all_jobs'])
        print '平均带权周转时间:%.2f' % (dic_time['all_wtr_time'] / dic_time['all_jobs'])

#hrrf算法
def hrrf(sys_time):
    while list:
        excellent = []      #临时存储响应比
        reach_time = []     #临时存储到达时间

        #计算响应比
        for i in range(0,len(list)):
            list[i]['wait_time'] = sys_time - list[i]['reach_time']
            list[i]['excellent'] = 1 + list[i]['wait_time']/list[i]['need_time']
            excellent.append(list[i]['excellent'])
            reach_time.append(list[i]['reach_time'])

        if excellent and max(excellent)<1:
            sys_time = min(reach_time)
            hrrf(sys_time)
        else:
            for i in range(0,len(list)):
                if list[i]['excellent'] == max(excellent):
                    tag = i
            #开始时间
            list[tag]['start_time'] = sys_time
            #等待时间
            list[tag]['wait_time'] = list[tag]['start_time'] - list[tag]['reach_time']
            dic_time['all_wait_time'] = dic_time['all_wait_time'] + list[tag]['wait_time']
            #周转时间
            list[tag]['tr_time'] = list[tag]['wait_time'] + list[tag]['need_time']
            dic_time['all_tr_time'] = dic_time['all_tr_time'] + list[tag]['tr_time']
            #带权周转时间
            list[tag]['wtr_time'] = list[tag]['tr_time'] / list[tag]['need_time']
            dic_time['all_wtr_time'] = dic_time['all_wtr_time'] + list[tag]['wtr_time']
            #更新系统时间
            sys_time = sys_time + list[tag]['need_time']
            #输出
            output(list[tag]['number'])
            #移除
            remove_jobs(list[tag]['number'])
            hrrf(sys_time)

initial_jobs()
read_jobdata()
hrrf(400)