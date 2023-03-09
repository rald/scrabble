class Demo {
	Sock	sock;
	DragArea	dragArea;
	Board	board;
	Rack		rack;
	
	Demo () {
		sock = new Sock();
		dragArea = new DragArea();
		TestFrame tf = new TestFrame("scrabble demo", dragArea);
		board = new Board();
		rack = new Rack(sock);
	}
	
	void play() {
		while(true) {
			System.out.println("rack: " + rack);
			Move m = board.findMove(rack);
			System.out.println("move: " + m);
			Assert.assert(m.score >=0);
			if(m.score==0)
				break;
			board.place(m);
			dragArea.addToBoard(m);
			rack.refresh(m);

		}
	}
	
	public static void main(String[] argv) {
		Demo d = new Demo();
		try {
			d.play();
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
