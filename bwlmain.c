/ *   h t t p : / / w i k i . t e a m l i q u i d . n e t / s t a r c r a f t / B W L 4 _ P l u g i n _ F o r m a t _ S p e c i f i c a t i o n   * /  
  
 # i n c l u d e   " r e s o u r c e s . h "  
 # i n c l u d e   " s q l o g . h "  
 # i n c l u d e   < w i n d o w s . h >  
 # i n c l u d e   < s t r i n g . h >  
  
 / /   S a m p l e   B W L 4   P l u g i n  
 / /   B O O L   i s   a   3 2 B i t   V a l u e  
 / /   A l l   B W L 4   f u n c t i o n s   a r e   C - C a l l i n g C o n v e n t i o n ( c d e c l )   n o t   s t d c a l l  
  
 # d e f i n e   B W L A P I   4  
 # d e f i n e   S T A R C R A F T B U I L D   1 3  
  
 / *     S T A R C R A F T B U I L D  
       - 1       A l l  
         0       1 . 0 4  
         1       1 . 0 8 b  
         2       1 . 0 9 b  
         3       1 . 1 0  
         4       1 . 1 1 b  
         5       1 . 1 2 b  
         6       1 . 1 3 f  
         7       1 . 1 4  
         8       1 . 1 5  
         9         1 . 1 5 . 1  
         1 0         1 . 1 5 . 2  
         1 1         1 . 1 5 . 3  
         1 2         1 . 1 6 . 0  
         1 3         1 . 1 6 . 1  
 * /  
  
 s t r u c t   E x c h a n g e D a t a  
 {  
 	 i n t   i P l u g i n A P I ;  
 	 i n t   i S t a r C r a f t B u i l d ;  
 	 B O O L   b N o t S C B W m o d u l e ;                                 / / I n f o r m   u s e r   t h a t   c l o s i n g   B W L   w i l l   s h u t   d o w n   y o u r   p l u g i n  
 	 B O O L   b C o n f i g D i a l o g ;                                   / / I s   C o n f i g u r a b l e  
 } ;  
  
 / /   B O O L   A P I E N T R Y   D l l M a i n (   H M O D U L E   h M o d u l e ,  
 / /                                                 D W O R D     u l _ r e a s o n _ f o r _ c a l l ,  
 / /                                                 L P V O I D   l p R e s e r v e d  
 / /                                   )  
 / /   {  
 / /   	 / / I s   t h i s   D L L   a l s o   S t a r C r a f t   m o d u l e ?  
 	  
 / /   	 s w i t c h   ( u l _ r e a s o n _ f o r _ c a l l )  
 / /   	 {  
 / /   	 	 c a s e   D L L _ P R O C E S S _ A T T A C H :  
 / /   	 	 	 M e s s a g e B o x ( N U L L ,   " S t a r C r a f t   m e s s a g e b o x " ,   " H u e " ,   M B _ O K ) ;  
 / /   	 	 	 r e t u r n   T R U E ;  
 / /   	 	 c a s e   D L L _ T H R E A D _ A T T A C H :  
 / /   	 	 c a s e   D L L _ T H R E A D _ D E T A C H :  
 / /   	 	 c a s e   D L L _ P R O C E S S _ D E T A C H :  
 / /   	 	 	 b r e a k ;  
 / /   	 }  
 	  
 / /   	 r e t u r n   T R U E ;  
 / /   }  
  
  
 H A N D L E   C r e a t e R e m o t e T h r e a d I n j e c t ( D W O R D   I D ,   c o n s t   w c h a r _ t *   d l l ) ;  
  
 / /  
 / /   G e t P l u g i n A P I   a n d   G e t D a t a   g e t   c a l l e d   d u r i n g   t h e   s t a r t u p   o f   t h e   L a u n c h e r   a n d   g a t h e r   i n f o r m a t i o n   a b o u t   t h e   p l u g i n  
 / /  
 e x t e r n   _ _ d e c l s p e c ( d l l e x p o r t )   v o i d   G e t P l u g i n A P I ( s t r u c t   E x c h a n g e D a t a   * D a t a )  
 {  
 	 / /   B W L   G e t s   v e r s i o n   f r o m   R e s o u r c e   -   V e r s i o n I n f o  
 	 D a t a - > i P l u g i n A P I   =   B W L A P I ;  
 	 D a t a - > i S t a r C r a f t B u i l d   =   S T A R C R A F T B U I L D ;  
 	 D a t a - > b C o n f i g D i a l o g   =   F A L S E ;  
 	 D a t a - > b N o t S C B W m o d u l e   =   T R U E ;  
 }  
  
 e x t e r n   _ _ d e c l s p e c ( d l l e x p o r t )   v o i d   G e t D a t a ( c h a r   * n a m e ,   c h a r   * d e s c r i p t i o n ,   c h a r   * u p d a t e u r l )  
 {  
 	 / /   i f   n e c e s s a r y   y o u   c a n   a d d   I n i t i a l i z e   f u n c t i o n   h e r e  
 	 / /   p o s s i b l y   c h e c k   C u r r e n t C u l t u r e   ( C u l t u r e I n f o )   t o   l o c a l i z e   y o u r   D L L   d u e   t o   s y s t e m   s e t t i n g s  
 	 s t r c p y ( n a m e ,             " S u p e r Q u i t t e r " ) ;  
 	 s t r c p y ( d e s c r i p t i o n ,  
 	 	 " S u p e r Q u i t t e r   f o r   1 . 1 6 . 1   V e r s i o n   " _ V E R S I O N " \ r \ n "  
 	 	 " E n a b l e s   q u i t   g a m e   f o r   a n y   c o n d i t i o n   w i t h   j u s t   p r e s s   c t r l + q + q \ r \ n "  
 	 	 " h t t p s : / / g i t h u b . c o m / m i g h t y 1 2 3 1 / S u p e r Q u i t t e r \ r \ n \ r \ n "  
 	 	 " b y   m i g h t y 1 2 3 1 " ) ;  
 	 s t r c p y ( u p d a t e u r l ,       " h t t p s : / / g i t h u b . c o m / m i g h t y 1 2 3 1 / S u p e r Q u i t t e r " ) ;  
 }  
  
 / /  
 / /   C a l l e d   w h e n   t h e   u s e r   c l i c k s   o n   C o n f i g  
 / /  
 e x t e r n   _ _ d e c l s p e c ( d l l e x p o r t )   B O O L   O p e n C o n f i g ( v o i d )  
 {  
 	 / /   I f   y o u   s e t   " D a t a . b C o n f i g D i a l o g   =   T R U E ; "   a t   f u n c t i o n   G e t P l u g i n A P I   t h e n  
 	 / /   B W L a u n c h e r   w i l l   c a l l   t h i s   f u n c t i o n   i f   u s e r   c l i c k s   C o n f i g   b u t t o n  
  
 	 / /   Y o u ' l l   n e e d   t o   m a k e   y o u r   o w n   W i n d o w   h e r e  
 	 r e t u r n   T R U E ;   / / e v e r y t h i n g   O K  
 }  
  
 / /  
 / /   A p p l y P a t c h S u s p e n d e d   a n d   A p p l y P a t c h   g e t  
 / /   c a l l e d   d u r i n g   t h e   s t a r t u p   o f   S t a r c r a f t   i n   t h e   L a u n c h e r   p r o c e s s  
 / /   t h e   h P r o c e s s   p a s s e d   t o   t h e m   i s   s h a r e d   b e t w e e n   a l l   p l u g i n s ,   s o   d o n ' t   c l o s e   i t .  
 / /   B e s t   p r a c t i c e   i s   d u p l i c a t i n g ( D u p l i c a t e H a n d l e   f r o m   W i n 3 2   A P I )   i t   i f   y o u   w a n t   t o   u s e   i f   a f t e r   t h e s e   f u n c t i o n   r e t u r n s  
  
 e x t e r n   _ _ d e c l s p e c ( d l l e x p o r t )   B O O L   A p p l y P a t c h S u s p e n d e d ( H A N D L E   h P r o c e s s ,   D W O R D   d w P r o c e s s I D )  
 {  
 	 / /   T h i s   f u n c t i o n   i s   c a l l e d   i n   t h e   L a u n c h e r   p r o c e s s   w h i l e   S t a r c r a f t   i s   s t i l l   s u s p e n d e d  
 	 / /   D u r n i n g   t h e   s u s p e n d e d   p r o c e s s   s o m e   m o d u l e s   o f   s t a r c r a f t . e x e   m a y   n o t   y e t   e x i s t .  
  
 	 / /   T h i s   f u n c t i o n   i s   c a l l e d   i n   t h e   L a u n c h e r   p r o c e s s   a f t e r   t h e   S t a r c r a f t   w i n d o w   h a s   b e e n   c r e a t e d  
 	 r e t u r n   T R U E ;   / / e v e r y t h i n g   O K  
 }  
  
 e x t e r n   _ _ d e c l s p e c ( d l l e x p o r t )   B O O L   A p p l y P a t c h ( H A N D L E   h P r o c e s s ,   D W O R D   d w P r o c e s s I D )  
 {  
 	 v o i d   * t o k e n H a n d l e ;  
 	 T O K E N _ P R I V I L E G E S   p r i v i l e g e T o k e n ;  
 	 w c h a r _ t   d l l [ M A X _ P A T H ] ;  
 	 H A N D L E   h T h r e a d ;  
 	 D W O R D   e x i t C o d e ;  
  
 	 O p e n P r o c e s s T o k e n (   G e t C u r r e n t P r o c e s s ( ) ,   T O K E N _ A D J U S T _ P R I V I L E G E S ,   & t o k e n H a n d l e   ) ;  
 	 L o o k u p P r i v i l e g e V a l u e (   0 ,   S E _ D E B U G _ N A M E ,   & p r i v i l e g e T o k e n . P r i v i l e g e s [ 0 ] . L u i d   ) ;  
 	 p r i v i l e g e T o k e n . P r i v i l e g e C o u n t   =   1 ;  
 	 p r i v i l e g e T o k e n . P r i v i l e g e s [ 0 ] . A t t r i b u t e s   =   S E _ P R I V I L E G E _ E N A B L E D ;  
 	 A d j u s t T o k e n P r i v i l e g e s (   t o k e n H a n d l e ,   0 ,   & p r i v i l e g e T o k e n ,   s i z e o f (   T O K E N _ P R I V I L E G E S   ) ,   0 ,   0   ) ;  
 	 i f (   ! C l o s e H a n d l e (   t o k e n H a n d l e   )   )   {  
 	 	 s q l o g ( L " ��:   t o k e n   �0�T�  ��(�" ) ;  
 	 	 r e t u r n   F A L S E ;  
 	 }  
  
 	 G e t F u l l P a t h N a m e W ( L " S u p e r Q u i t t e r . b w l " ,   M A X _ P A T H ,   d l l ,   N U L L ) ;  
 	 h T h r e a d   =   C r e a t e R e m o t e T h r e a d I n j e c t ( d w P r o c e s s I D ,   d l l ) ;  
 	 i f   (   h T h r e a d   ! =   0   )   {  
 	 	 s q l o g ( L " ��:   C r e a t e R e m o t e T h r e a d   1���" ) ;  
 	 	 W a i t F o r S i n g l e O b j e c t (   h T h r e a d ,   I N F I N I T E   ) ;  
 	 	 G e t E x i t C o d e T h r e a d (   h T h r e a d ,   & e x i t C o d e   ) ;  
 	 	 C l o s e H a n d l e (   h T h r e a d   ) ;  
 	 }   e l s e   {  
 	 	 s q l o g ( L " ��:   C r e a t e R e m o t e T h r e a d   ��(�" ) ;  
 	 	 r e t u r n   F A L S E ;  
 	 }  
 	 i f   ( e x i t C o d e   = =   0 )   {  
 	 	 s q l o g ( L " ��:   L o a d L i b r a r y   ��(�" ) ;  
 	 	 r e t u r n   F A L S E ;  
 	 }  
  
 	 r e t u r n   T R U E ;   / / e v e r y t h i n g   O K  
 }  
  
 H A N D L E   C r e a t e R e m o t e T h r e a d I n j e c t ( D W O R D   I D ,   c o n s t   w c h a r _ t *   d l l )  
 {  
 	 H A N D L E   P r o c e s s ,   T h r e a d ;  
 	 L P V O I D   M e m o r y ;  
 	 L P V O I D   L o a d L i b r a r y ;  
 	 w c h a r _ t   m s g t e m p [ 2 5 5 ] ;  
  
 	 i f   ( I D   = =   0 )  
 	 {  
 	 	 r e t u r n   N U L L ;  
 	 }  
  
 	 P r o c e s s   =   O p e n P r o c e s s ( P R O C E S S _ C R E A T E _ T H R E A D   |   P R O C E S S _ Q U E R Y _ I N F O R M A T I O N   |   P R O C E S S _ V M _ R E A D   |   P R O C E S S _ V M _ W R I T E   |   P R O C E S S _ V M _ O P E R A T I O N ,   F A L S E ,   I D ) ;  
 	 i f   ( P r o c e s s   = =   N U L L )  
 	 {  
 	 	 s w p r i n t f _ s ( m s g t e m p ,   s i z e o f ( m s g t e m p ) ,   L " O p e n P r o c e s s   r e t u r n e d   % d " ,   G e t L a s t E r r o r ( ) ) ;  
 	 	 r e t u r n   N U L L ;  
 	 }  
  
 	 L o a d L i b r a r y   =   ( L P V O I D ) G e t P r o c A d d r e s s ( G e t M o d u l e H a n d l e A ( " k e r n e l 3 2 . d l l " ) ,   " L o a d L i b r a r y A " ) ;  
 	 i f   ( L o a d L i b r a r y   = =   N U L L )  
 	 {  
 	 	 s w p r i n t f _ s ( m s g t e m p ,   s i z e o f ( m s g t e m p ) ,   L " G e t P r o c A d d r e s s   r e t u r n e d   % d " ,   G e t L a s t E r r o r ( ) ) ;  
 	 	 C l o s e H a n d l e ( P r o c e s s ) ;  
 	 	 r e t u r n   N U L L ;  
 	 }  
  
 	 c h a r   m b s [ M A X _ P A T H ] ;  
 	 W i d e C h a r T o M u l t i B y t e ( C P _ A C P ,   W C _ C O M P O S I T E C H E C K ,   d l l ,   - 1 ,   m b s ,   s i z e o f ( m b s ) ,   N U L L ,   N U L L ) ;  
  
 	 M e m o r y   =   ( L P V O I D ) V i r t u a l A l l o c E x ( P r o c e s s ,   N U L L ,   s t r l e n ( m b s )   +   1 ,   M E M _ R E S E R V E   |   M E M _ C O M M I T ,   P A G E _ R E A D W R I T E ) ;  
 	 i f   ( M e m o r y   = =   N U L L )  
 	 {  
 	 	 s w p r i n t f _ s ( m s g t e m p ,   s i z e o f ( m s g t e m p ) ,   L " V i r t u a l A l l o c E x   r e t u r n e d   % d " ,   G e t L a s t E r r o r ( ) ) ;  
 	 	 C l o s e H a n d l e ( P r o c e s s ) ;  
 	 	 r e t u r n   N U L L ;  
 	 }  
  
 	 W r i t e P r o c e s s M e m o r y ( P r o c e s s ,   ( L P V O I D ) M e m o r y ,   m b s ,   s t r l e n ( m b s )   +   1 ,   N U L L ) ;  
  
 	 T h r e a d   =   C r e a t e R e m o t e T h r e a d ( P r o c e s s ,   N U L L ,   N U L L ,   ( L P T H R E A D _ S T A R T _ R O U T I N E ) L o a d L i b r a r y ,   ( L P V O I D ) M e m o r y ,   N U L L ,   N U L L ) ;  
  
 	 C l o s e H a n d l e ( P r o c e s s ) ;  
  
 	 V i r t u a l F r e e E x ( P r o c e s s ,   ( L P V O I D ) M e m o r y ,   0 ,   M E M _ R E L E A S E ) ;  
  
 	 r e t u r n   T h r e a d ;  
 } 