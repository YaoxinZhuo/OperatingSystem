package rr;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

public class RR {
	private static final String FILE_IN = "C:\\Users\\River\\Desktop\\processinfo.txt";
	private static BlockingQueue<RRNode> taskQueue = new LinkedBlockingQueue<RRNode>(); // 进程执行队列
	private static List<RRNode> tasklist = new ArrayList<>(); // 任务节点存放List
	private static int sys_time = 700; // 系统当前时间
	private static final int rr_time = 10; // 时间片
	private static int tasknum = 7; // 进程数量

	public static void main(String[] args) {
		System.out.println("当前时间片大小:"+rr_time+"   系统时间:"+sys_time);
		List<String> taskinfo = readInfo();
		//System.out.println(taskinfo);
		tasklist = createRRNode(taskinfo);
		initTasklist();
		while (tasknum > 0) {
			addTask();
			run();
			moveTask();
		}
	}

	private static void run() {
		sys_time += rr_time;
	}

	private static void updateInfo(RRNode node) { // 每次时间片运行结束执行，更新当前节点的信息
		if (node.isreached) { // 作业已完成 直接返回
			return;
		}

		else if (node.visited) { // 非首次执行
			updateTime(node);
		} else { // 首次执行
			node.visited = true;
			node.start_time = sys_time;
			updateTime(node);
		}

	}

	private static void updateTime(RRNode node) { // 更新节点时间相关属性
		node.run_time += rr_time;
		if (node.run_time >= node.need_time) {
			int end_time = sys_time - (node.run_time-node.need_time);
			
			node.wait_time = end_time - node.reach_time - node.need_time;
			node.tr_time = end_time - node.reach_time; // 如果是已经完成任务的节点不会到这里
														// 直接在updateInfo就被返回了
			node.wtr_time = node.tr_time / node.need_time;	
			node.excellent = (node.wait_time + node.need_time) / node.need_time;
			
			node.run_time = node.need_time;
			node.isreached = true;
			System.out.println("结束运行"+node.number);
		}
		else{
			System.out.println("暂停运行"+node.number);
			
			node.wait_time = sys_time - node.reach_time - node.run_time;
			node.tr_time = sys_time - node.reach_time; // 如果是已经完成任务的节点不会到这里
														// 直接在updateInfo就被返回了
			node.wtr_time = node.tr_time / node.need_time;			
			node.excellent = (node.wait_time + node.need_time) / node.need_time;
		}

	}

	private static void moveTask() { // 每次时间片运行结束执行，检测节点状态，若未完成则将队列头部移至队列尾，完成则移除并计数器--
		RRNode node = taskQueue.poll();
		if(node == null)
			return;
		System.out.println("开始运行"+node.number);
		updateInfo(node);
		if (node.isreached) {
			tasknum--;
			printInfo(node);	//打印完成进程信息
			return;
		}
		addTask();		//懒得改了 多执行一遍而已 这里如果先加入队列再把头结点移动到队尾 不然会出问题
		taskQueue.add(node);
	}

	private static void addTask() { // 每次进程时间片执行完毕执行此方法,检测是否有新的进程到达并将其加入任务队列
		for (RRNode node : tasklist) {
			if (node.reach_time <= sys_time && node.visited == false && !taskQueue.contains(node)) {
				taskQueue.add(node);
				System.out.println("进程"+node.number+"到达   当前系统时间:"+sys_time);
			}
		}
	}
	
	private static void printInfo(RRNode node){
		System.out.println("***进程"+node.number+"已完成***  当前系统时间:"+sys_time+"   所需执行时间为"+node.need_time);
		System.out.println("等待时间："+node.wait_time);
		System.out.println("周转时间:"+node.tr_time);
		System.out.println("带权周转时间:"+node.wtr_time);
	}
	
	private static List<RRNode> createRRNode(List<String> tmp) { // 创建进程节点
		List<RRNode> nodelist = new ArrayList<>();
		for (String a : tmp) {
			RRNode node = new RRNode(Integer.parseInt(a.split("\t")[0]),
					Integer.parseInt(a.split("\t")[1]), Integer.parseInt(a
							.split("\t")[2]), 0); // 不涉及优先级默认为0
			nodelist.add(node);
		}
		return nodelist;
	}

	private static void initTasklist() { // 按照reach_time对List内进行排序
		tasklist = RRNode.initTasklist(tasklist);
//		for(RRNode node:tasklist){
//			System.out.println(node.number+" "+node.reach_time+" "+node.need_time);
//		}
	}

	private static List<String> readInfo() { // 读取文件信息
		List<String> tmplist = new ArrayList<>();
		String tmp = null;
		String[] tmparray = null;
		int len = 0;
		StringBuffer str = new StringBuffer("");
		File file = new File(FILE_IN);
		try {
			FileInputStream is = new FileInputStream(file);
			InputStreamReader isr = new InputStreamReader(is);
			BufferedReader in = new BufferedReader(isr);
			String line = null;
			while ((line = in.readLine()) != null) {
				if (len != 0) // 处理换行符的问题
				{
					str.append("\r\n" + line);
				} else {
					str.append(line);
				}
				len++;
			}
			in.close();
			is.close();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		tmp = str.toString().replaceAll(" ", "");
		tmparray = tmp.split("\r\n");
		for (String a : tmparray) {
			tmplist.add(a);
		}
		return tmplist;
	}
}
