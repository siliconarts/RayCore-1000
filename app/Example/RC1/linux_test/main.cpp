#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h> 
#include <unistd.h>

Window *winlist (Display *disp, unsigned long *len);
char *winame (Display *disp, Window win);  
void processEvent(Display *disp, Window window, unsigned int width, unsigned int height, int scrnum);
XImage *CreateTrueColorImage(Display *, Visual *, char *, int, int);

int main(int argc, char *argv[]) {    
	Display *disp = 0; //= XOpenDisplay(NULL);     

	//window config
	XImage *ximage;
	unsigned int width=800, height=480;
	int scrnum;
	unsigned int border_width=2;
	Window root;
	Window window;
	char *dpyName	= NULL;
	int	x=50,y=50;
	int m_width, m_height;
	XWindowAttributes windowattr;

	//create pixmap data test
	char *image;

	//window open
	disp	= XOpenDisplay(dpyName);

	if (!disp) {        
		printf("Error : couldn't open display %s\n",
			dpyName	? dpyName : getenv("DISPLAY"));        
		return -1; 
	}     
	DestroyNotify
	scrnum	= DefaultScreen(disp);
	
	root = XDefaultRootWindow (disp);

	//window = XCreateSimpleWindow ( disp, root, x, y, width, height,
	//							border_width, BlackPixel (disp,0), WhitePixel(disp,0) );

	//m_width			= DisplayWidth(disp, scrnum);
	//m_height		= DisplayHeight(disp, scrnum);
	//printf("width : %d, height : %d\n", m_width, m_height);

	//pixmap test start
	Visual *visual=DefaultVisual(disp, 0);
	window=XCreateSimpleWindow(disp, root, x, y, width, height, border_width, BlackPixel (disp,0), WhitePixel(disp,0));
	if(visual->c_class!=TrueColor)
	{
		fprintf(stderr, "Cannot handle non true color visual ...\n");
		exit(1);
	}

	//ximage=CreateTrueColorImage(disp, visual, 0, width, height);
	//XSelectInput(disp, window, ButtonPressMask|ExposureMask);
	//test end

	if (XGetWindowAttributes(disp, window,
		&windowattr) == 0) 
	{
		perror("%s: can't get root window geometry\n");
		exit(-1);
	}
	m_width = windowattr.width;
	m_height = windowattr.height;
	printf("width : %d, height : %d\n", m_width, m_height);

	XMapWindow(disp, window);

	//CreateTrueColorImage(disp, visual, unsigned char *image, int width, int height);
	//ximage = CreateTrueColorImage(disp, visual,0, width, height);
	//XPutImage(disp, window, DefaultGC(disp, 0), ximage, 0, 0, 0, 0, width, height);
	//XDestroyImage(ximage);

	

	//key event while¹®
	processEvent(disp, window, width, height, scrnum);
	//sleep(3);
	
	////get window list
	//list = (Window*)winlist(disp,&len);     

	//for (i=0;i<(int)len;i++) {        
	//	name = winame(disp,list[i]);    
	//	printf("-->%s<--\n",name);        
	//	free(name);
	//}     

	//XFree(list);   
	
	XCloseDisplay(disp);    
	return 0;
}  

Window *winlist (Display *disp, unsigned long *len) {    
	Atom prop = XInternAtom(disp,"_NET_CLIENT_LIST",False), type;    
	int form;    
	unsigned long remain;    
	unsigned char *list;     
	errno = 0;    

	if (XGetWindowProperty(disp,XDefaultRootWindow(disp),prop,0,1024,False,XA_WINDOW,
		&type,&form,len,&remain,&list) != Success) 
	{        perror("winlist() -- GetWinProp\n");        
	return 0;    
	}         

	return (Window*)list;
}  

char *winame (Display *disp, Window win) {    
	Atom prop = XInternAtom(disp,"WM_NAME",False), type;    
	int form;    
	unsigned long remain, len;    
	unsigned char *list;     
	errno = 0;    

	if (XGetWindowProperty(disp,win,prop,0,1024,False,XA_STRING,                
		&type,&form,&len,&remain,&list) != Success) {        
			perror("winlist() -- GetWinProp\n");        
			return NULL;    
	}    

	return (char*)list;
}

void processEvent(Display *disp, Window window, unsigned int width, unsigned int height, int scrnum) {
	int i;
	unsigned long len;    
	Window *list; 
	char *name; 
	int redraw = 0;
	char *image;
	XImage *ximage;
	Visual *visual = DefaultVisual(disp, scrnum);

	list = (Window*)winlist(disp,&len);     

	for (i=0;i<(int)len;i++) {        
		name = winame(disp,list[i]);    
		printf("-->%s<--\n",name);        
		free(name);
	}     

	XFree(list); 
// 	ximage = CreateTrueColorImage(disp, visual,0, width, height);
// 	XPutImage(disp, window, DefaultGC(disp, 0), ximage, 0, 0, 0, 0, width, height);
// 	XDestroyImage(ximage);

	while(1) {
		XEvent ev;
		XNextEvent(disp, &ev);    
	    switch(ev.type)
	    {	
	    case Expose:
			/*
        	XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
	        XSetForeground(display, DefaultGC(display, 0), 0x0000ffff); // red
	
        	XDrawString(display, window, DefaultGC(display, 0), 32,     32,     tir, strlen(tir));
			*/

			printf("Expose\n");
			printf("redraw is 1\n");
			
			
			
			
			redraw = 1;
        	break;
		case KeyPress:
			{
				char buffer[10];
				int r, code;

				code = XLookupKeysym(&ev.xkey, 0);
				switch(code) {
					case XK_Left:
					case XK_Right:
					case XK_Up:
					case XK_Down:
						//get window list
						redraw = 2;
						printf("redraw is 2\n");
					default:
						r = XLookupString(&ev.xkey, buffer, sizeof(buffer), NULL, NULL);
						if(buffer[0] = 27) return;
				}
			}
			break;
	   default :
		   ;
    	}

		if(redraw == 1) {
			printf("redraw is 11111\n");
			XFlush (disp);
		}
		else if(redraw == 2) {
			printf("redraw is 22222\n");
			list = (Window*)winlist(disp,&len);     

			for (i=0;i<(int)len;i++) {        
				name = winame(disp,list[i]);    
				printf("-->%s<--\n",name);        
				free(name);
			}     

			XFree(list);   
			
			redraw = 0;
		}
		else
			if(image != 0) free(image);
		

    }
}

//test pixmap
XImage *CreateTrueColorImage(Display *display, Visual *visual, char *image, int width, int height)
{
	int i, j;
	char *image32=(char *)malloc(width*height*4);
	char *p=image32;
	unsigned int r=0xff, g=0xff, b=0x00;

	printf("CreateTrueColorImage loaded.....\n");

	for(i=0; i<height; i++)
	{
		for(j=0; j<width; j++)
		{
			*p++=b;
			*p++=g;
			*p++=r;
			p++;
		}
	}
	return XCreateImage(display, visual, 24, ZPixmap, 0, image32, width, height, 32, 0);
}