/* ripple carry adder */
module addern(carryin, X, Y, S, carryout);
	parameter n = 32;
	input carryin;
	input [n-1:0] X, Y;
	output [n-1:0] S;
	output carryout;
	wire [n:0] C;
	wire [n-1:0] z1, z2, z3;

	assign C[0] = carryin;
	assign carryout = C[n];
	xor x0[n-1:0] (S, X, Y, C[n-1:0]);
	and a0[n-1:0] (z1, X, Y);
	and a1[n-1:0] (z2, X, C[n-1:0]);
	and a2[n-1:0] (z3, Y, C[n-1:0]);
	or o0[n-1:0] (C[n:1], z1, z2, z3);
endmodule

module compar1(AeqB, AgtB, A, B, AeqBin, AgtBin);
	input A, B, AeqBin, AgtBin;
	output AeqB, AgtB;
	wire eq0, gt0;

	// A == B when A and B are same, and all more significant bits are same
	and (AeqB, AeqBin, eq0);
	xnor (eq0, A, B);

	// A > B when A == 1 and B == 0 and more significant bits were same
	// or if more significant bits where A > B
	or(AgtB, AgtBin, gt0);
	and(gt0, AeqBin, A, ~B);
endmodule

module alarm_struct(alrm, day2, day1, day0);  //don't forget the ';'

	input day2, day1;
	input day0;
	output alrm;
	wire nd2, nd1, nd0;
	wire p1, p2;

	// structural code lists components and connections
	not(nd2, day2);
	not(nd1, day1);
	not(nd0, day0);
	and(p1, nd2, nd0);
	and(p2, nd1, nd0);
	or(alrm, p1, p2);
endmodule
