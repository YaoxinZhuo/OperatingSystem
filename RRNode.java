package rr;

import java.util.Collections;
import java.util.List;

public class RRNode implements Comparable<RRNode> {
	int number; // 作业号
	int reach_time; // 作业抵达时间
	int need_time; // 作业的执行时间
	int privilege; // 作业优先权

	int start_time; // 作业开始时间

	float excellent; // 响应比 (wait_time+need_time)/need_time
	int wait_time; // 等待时间
	int tr_time; // 周转时间 开始进入到结束时间
	double wtr_time; // 带权周转时间 周转时间/执行时间
	int run_time; // 作业累计已执行时间
	boolean visited; // 作业是否被访问过
	boolean isreached; // 作业是否已抵达 #########################这个属性被我当成进程结束标志用了

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

	public static List<RRNode> initTasklist(List<RRNode> list) { // 按照reach_time对List内进行排序
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
