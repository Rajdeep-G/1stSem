//Q1

#include<stdio.h>
#include<math.h>

struct circle{
	struct point{
	int x;
	int y;
	};
	struct point p;
	int radius;
};
int main(){
	int choice;
	printf("Press 1 for task 2 and press 2 for taks 3");
	scanf("%d",&choice);
	if (choice==1){
		struct circle c;
		int x,y;
		printf("Input a point");
		printf("\nEnter X cordinate:");
		scanf("%d",&x);
		printf("Enter y cordinate:");
		scanf("%d",&y);
		
		printf("\nInput point is : P(%d,%d)",x,y);
		
		
		printf("\nEnter X cordinate of center:");
		scanf("%d",&(c.p.x));
		printf("Enter y cordinate of center:");
		scanf("%d",&(c.p.y));
		printf("Enter the radius");
		scanf("%d",&(c.radius));
		
		printf("\nInput Circle is: C[P(%d,%d),%d]",c.p.x,c.p.y,c.radius);
		
		int f = (((c.p.y)-y)*((c.p.y)-y)) + (((c.p.x)-x)*((c.p.x)-x));
		//printf("%d",f);
		int ans=-1;
		double dist=sqrt(f);
		if(c.radius==dist)
			ans=0;
		else if (c.radius<dist)
			ans=1;
		
		printf("\nRelation of point with circle is: Point lies ");
		if(ans==0)
			printf("on");
		else if(ans==1)
			printf("outside");
		else if (ans==-1)
			printf("inside");
		printf(" the circle\n");
		
		printf("....................................................");
	
	
	}
	else if (choice ==2){
	
	
		printf("\nInput details of circle C1");
		struct circle c1,c2;
		
		printf("\nEnter X cordinate of center:");
		scanf("%d",&(c1.p.x));
		printf("Enter y cordinate of center:");
		scanf("%d",&(c1.p.y));
		printf("Enter the radius");
		scanf("%d",&(c1.radius));
		
		printf("\nInput Circle is: C[P(%d,%d),%d]",c1.p.x,c1.p.y,c1.radius);
		
		
		
		
		printf("\nInput details of circle C2");

		
		printf("\nEnter X cordinate of center:");
		scanf("%d",&(c2.p.x));
		printf("Enter y cordinate of center:");
		scanf("%d",&(c2.p.y));
		printf("Enter the radius");
		scanf("%d",&(c2.radius));
		
		printf("\nInput Circle is: C[P(%d,%d),%d]",c2.p.x,c2.p.y,c2.radius);
		
		printf("\nRelation of point with circle is:  ");
		
		double dis_between_centres=sqrt(((c1.p.x-c2.p.x)*(c1.p.x-c2.p.x))  + ((c1.p.y-c2.p.y)*(c1.p.y-c2.p.y)) );
		
		int diff_between_rad=abs(c1.radius-c2.radius);
		int sum_rad=c1.radius+c2.radius;
		
		if (dis_between_centres<diff_between_rad)
			printf("C1 lies inside C2 or C2 inside C1");
		else if (dis_between_centres<sum_rad)
			printf("C1 C2 intersect each other");
		else if (dis_between_centres=sum_rad)
			printf("C1 C2 touch each other");
		else if (dis_between_centres>sum_rad)
			printf("C1 C2 are disjoint");
			
	}
	else
	printf("Incorrect input");	

	return 0;
}
