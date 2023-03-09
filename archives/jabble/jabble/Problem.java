import java.awt.Point;

/** Indicate a problem with a proposed scrabble move **/
class Problem {
	String message;
	Point	p1;
	Point	p2;
	
	Problem(String s, Point p1, Point p2) {
		message = s;
		this.p1 = p1;
		this.p2 = p2;
	}
	Problem(String s, Point p1) {
		this(s, p1, null);
	}
	Problem(String s) {
		this(s,null, null);
	}
}
