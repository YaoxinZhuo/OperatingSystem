package rr;

import java.util.Collections;
import java.util.List;

public class RRNode implements Comparable<RRNode> {
	int number; // ��ҵ��
	int reach_time; // ��ҵ�ִ�ʱ��
	int need_time; // ��ҵ��ִ��ʱ��
	int privilege; // ��ҵ����Ȩ

	int start_time; // ��ҵ��ʼʱ��

	float excellent; // ��Ӧ�� (wait_time+need_time)/need_time
	int wait_time; // �ȴ�ʱ��
	int tr_time; // ��תʱ�� ��ʼ���뵽����ʱ��
	double wtr_time; // ��Ȩ��תʱ�� ��תʱ��/ִ��ʱ��
	int run_time; // ��ҵ�ۼ���ִ��ʱ��
	boolean visited; // ��ҵ�Ƿ񱻷��ʹ�
	boolean isreached; // ��ҵ�Ƿ��ѵִ� #########################������Ա��ҵ��ɽ��̽�����־����

	RRNode(int number, int reach_time, int need_time, int privilege) {
		this.number = number;
		this.reach_time = reach_time;
		this.need_time = need_time;
		this.privilege = privilege;

		this.excellent = -1;
		this.start_time = -1;
		this.wait_time = -1;
		this.tr_time = -1;
		this.wtr_time = -1;
		this.run_time = 0;
		this.visited = false;
		this.isreached = false;

	}

	public static List<RRNode> initTasklist(List<RRNode> list) { // ����reach_time��List�ڽ�������
		Collections.sort(list);
//		for (RRNode rr : list)
//			System.out.println(rr.reach_time);
		return list;
	}

	public int compareTo(RRNode o) {
		if (this.reach_time > o.reach_time) {
			return 1;
		} else if(this.reach_time == o.reach_time) {
			return 0;
		}else{
			return -1;
		}

	}
}
