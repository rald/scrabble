import java.awt.event.*;
import java.awt.*;

class Jabble implements ActionListener {
	Sock	sock;
	DragArea	dragArea;
	Board	board;
	Rack		playerRack;
	Rack		computerRack;
	boolean	finished;
	Button	moveReadyButton, suggestMoveButton, restartButton;
	TextField	computerScoreField, playerScoreField;
	int		computerScore, playerScore;
	
	Jabble () {
		sock = new Sock();
		dragArea = new DragArea();
		TestFrame tf = new TestFrame("scrabble demo", dragArea);
		Panel top = topPanel();
		computerScore = 0;
		playerScore = 0;
		
		tf.add(top, "North");
		tf.pack();

		moveReadyButton.setEnabled(false);
		board = new Board();
		playerRack = new Rack(sock);
		computerRack = new Rack(sock);
		dragArea.addToRack(playerRack.getNewLetters());
		finished=false;
	}
	
	Panel topPanel() {
		Panel top = new Panel();
		
		computerScoreField = new TextField("0", 3);
		top.add(computerScoreField);
		
		moveReadyButton = new Button("moveReady");
		moveReadyButton.addActionListener(this);
		top.add(moveReadyButton);
		
		suggestMoveButton = new Button("suggestMove");
		suggestMoveButton.addActionListener(this);
		top.add(suggestMoveButton);
		
		restartButton = new Button("restart");
		restartButton.addActionListener(this);
		top.add(restartButton);
		
		playerScoreField = new TextField("0", 3);
		top.add(playerScoreField);
		
		return top;
	}
	
	/** Handle various button presses **/
	public void actionPerformed(ActionEvent e) {
		String s = e.getActionCommand();
		if(s.equals("moveReady")) {
			humanMove();
		} else if (s.equals("suggestMove")) {
			suggestMove();
		} else if (s.equals("restart")) {
			restart();
		} else {
			throw new RuntimeException("strange button");
		}
	}
	
	/** Get the computer to suggest a move we could make **/
	void suggestMove() {
		notyet();	// todo
	}
	
	/** Start a new game. **/
	void restart() {
		notyet();	// todo
	}
	
	void notyet() {
		System.out.println("not yet implemented");
	}
	
	void computerMove(){
		Move m = board.findMove(computerRack);
		if(m.score==0)
			System.exit(0);
		board.place(m);
		dragArea.addToBoard(m);
		computerScore += m.score;
		computerScoreField.setText(String.valueOf(computerScore));
		computerRack.refresh(m);
		moveReadyButton.setEnabled(true);
	}
	
	void humanMove() {
		Move m = dragArea.getMove();
		Problem p = board.checkMove(m);
		if(p==null){
			humanCommitMove(m);
		} else {
			dragArea.displayProblem(p);
		}
	}
	
	void humanCommitMove(Move m) {
		board.place(m);
		dragArea.commit();
		playerScore += m.score;
		playerScoreField.setText(String.valueOf(playerScore));
		playerRack.refresh(m);
		dragArea.addToRack(playerRack.getNewLetters());
		moveReadyButton.setEnabled(false);
		computerMove();
	}
	
	public static void main(String[] argv) {
		Jabble game = new Jabble();
		try {
			if (Math.random()<0.5)
				game.computerMove();
			else
				game.moveReadyButton.setEnabled(true);
		} catch(Exception e) {
			e.printStackTrace();
		}
	}
}
