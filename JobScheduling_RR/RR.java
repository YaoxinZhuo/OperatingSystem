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
	private static BlockingQueue<RRNode> taskQueue = new LinkedBlockingQueue<RRNode>(); // ����ִ�ж���
	private static List<RRNode> tasklist = new ArrayList<>(); // ����ڵ���List
	private static int sys_time = 700; // ϵͳ��ǰʱ��
	private static final int rr_time = 10; // ʱ��Ƭ
	private static int tasknum = 7; // ��������

	public static void main(String[] args) {
		System.out.println("��ǰʱ��Ƭ��С:"+rr_time+"   ϵͳʱ��:"+sys_time);
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

	private static void updateInfo(RRNode node) { // ÿ��ʱ��Ƭ���н���ִ�У����µ�ǰ�ڵ����Ϣ
		if (node.isreached) { // ��ҵ����� ֱ�ӷ���
			return;
		}

		else if (node.visited) { // ���״�ִ��
			updateTime(node);
		} else { // �״�ִ��
			node.visited = true;
			node.start_time = sys_time;
			updateTime(node);
		}

	}

	private static void updateTime(RRNode node) { // ���½ڵ�ʱ���������
		node.run_time += rr_time;
		if (node.run_time >= node.need_time) {
			int end_time = sys_time - (node.run_time-node.need_time);
			
			node.wait_time = end_time - node.reach_time - node.need_time;
			node.tr_time = end_time - node.reach_time; // ������Ѿ��������Ľڵ㲻�ᵽ����
														// ֱ����updateInfo�ͱ�������
			node.wtr_time = node.tr_time / node.need_time;	
			node.excellent = (node.wait_time + node.need_time) / node.need_time;
			
			node.run_time = node.need_time;
			node.isreached = true;
			System.out.println("��������"+node.number);
		}
		else{
			System.out.println("��ͣ����"+node.number);
			
			node.wait_time = sys_time - node.reach_time - node.run_time;
			node.tr_time = sys_time - node.reach_time; // ������Ѿ��������Ľڵ㲻�ᵽ����
														// ֱ����updateInfo�ͱ�������
			node.wtr_time = node.tr_time / node.need_time;			
			node.excellent = (node.wait_time + node.need_time) / node.need_time;
		}

	}

	private static void moveTask() { // ÿ��ʱ��Ƭ���н���ִ�У����ڵ�״̬����δ����򽫶���ͷ����������β��������Ƴ���������--
		RRNode node = taskQueue.poll();
		if(node == null)
			return;
		System.out.println("��ʼ����"+node.number);
		updateInfo(node);
		if (node.isreached) {
			tasknum--;
			printInfo(node);	//��ӡ��ɽ�����Ϣ
			return;
		}
		addTask();		//���ø��� ��ִ��һ����� ��������ȼ�������ٰ�ͷ����ƶ�����β ��Ȼ�������
		taskQueue.add(node);
	}

	private static void addTask() { // ÿ�ν���ʱ��Ƭִ�����ִ�д˷���,����Ƿ����µĽ��̵��ﲢ��������������
		for (RRNode node : tasklist) {
			if (node.reach_time <= sys_time && node.visited == false && !taskQueue.contains(node)) {
				taskQueue.add(node);
				System.out.println("����"+node.number+"����   ��ǰϵͳʱ��:"+sys_time);
			}
		}
	}
	
	private static void printInfo(RRNode node){
		System.out.println("***����"+node.number+"�����***  ��ǰϵͳʱ��:"+sys_time+"   ����ִ��ʱ��Ϊ"+node.need_time);
		System.out.println("�ȴ�ʱ�䣺"+node.wait_time);
		System.out.println("��תʱ��:"+node.tr_time);
		System.out.println("��Ȩ��תʱ��:"+node.wtr_time);
	}
	
	private static List<RRNode> createRRNode(List<String> tmp) { // �������̽ڵ�
		List<RRNode> nodelist = new ArrayList<>();
		for (String a : tmp) {
			RRNode node = new RRNode(Integer.parseInt(a.split("\t")[0]),
					Integer.parseInt(a.split("\t")[1]), Integer.parseInt(a
							.split("\t")[2]), 0); // ���漰���ȼ�Ĭ��Ϊ0
			nodelist.add(node);
		}
		return nodelist;
	}

	private static void initTasklist() { // ����reach_time��List�ڽ�������
		tasklist = RRNode.initTasklist(tasklist);
//		for(RRNode node:tasklist){
//			System.out.println(node.number+" "+node.reach_time+" "+node.need_time);
//		}
	}

	private static List<String> readInfo() { // ��ȡ�ļ���Ϣ
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
				if (len != 0) // �����з�������
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
