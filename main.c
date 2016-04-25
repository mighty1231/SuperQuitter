# i n c l u d e   " h o o k . h "  
 # i n c l u d e   " s q l o g . h "  
 # i n c l u d e   < s t d i o . h >  
 # i n c l u d e   < w c h a r . h >  
  
 / *  
   *   D l l M a i n   c a l l e d   m u l t i p l e   t i m e s ,  
   *   s i n c e   S t a r c r a f t   i s   m u l t i - t h r e a d e d   a p p l i c a t i o n ,  
   *   a n d   D l l M a i n   i s   c a l l e d   f o r   e v e r y   t h r e a d   o f   S t a r c r a f t .  
   *   * /  
 i n t   c a l l e d C o u n t   =   - 1 ;  
 B O O L   i s O n S t a r c r a f t   =   F A L S E ;  
  
 s t r u c t   w i n d o w I n f o   {  
 	 H W N D   h w n d ;  
 	 w c h a r _ t   c l a s s N a m e [ 2 0 ] ;  
 } ;  
  
 B O O L   C A L L B A C K   E n u m W i n d o w s P r o c _ g e t C l a s s N a m e ( H W N D   h w n d ,   L P A R A M   l P a r a m ) ;  
  
 B O O L   W I N A P I   D l l M a i n ( H I N S T A N C E   h I n s t ,   D W O R D   f d w R e a s o n ,   L P V O I D   l p R e s e r v e d )   {  
 	 D W O R D   o l d P r o t e c t ;  
 	 w c h a r _ t   m o d u l e N a m e [ M A X _ P A T H ] ;  
 	 H W N D   h a n d l e ;  
 	 w c h a r _ t   c l a s s N a m e [ 2 0 ] ;  
 	 / /   D W O R D   s t a r t T i c k ,   c u r T i c k ;  
 	 s w i t c h   ( f d w R e a s o n )   {  
 	 	 c a s e   D L L _ P R O C E S S _ A T T A C H :  
 	 	 	 b r e a k ;  
  
 	 	 c a s e   D L L _ P R O C E S S _ D E T A C H :  
 	 	 	 b r e a k ;  
  
 	 	 c a s e   D L L _ T H R E A D _ A T T A C H :  
 	 	 	 c a l l e d C o u n t + + ;  
 	 	 	 i f   ( c a l l e d C o u n t   = =   0 )   {  
 	 	 	 	 / *   h w n d   :   t h e   f i r s t   e n u m e r a t e   w i n d o w   o r   S W a r C l a s s   * /  
 	 	 	 	 s t r u c t   w i n d o w I n f o   w i ;  
 	 	 	 	 w i . h w n d   =   0 ;  
 	 	 	 	 E n u m W i n d o w s ( & E n u m W i n d o w s P r o c _ g e t C l a s s N a m e ,   ( L P A R A M )   & w i ) ;  
  
 	 	 	 	 i f   ( w i . h w n d   ! =   0   & &  
 	 	 	 	 	 w c s c m p ( w i . c l a s s N a m e ,   L " S W a r C l a s s " )   = =   0 ) {  
 	 	 	 	 	 i s O n S t a r c r a f t   =   T R U E ;  
 	 	 	 	 }   e l s e   {  
 	 	 	 	 	 s q l o g ( L " ���:   ����  t�x�X�  �\�8��� �  d l l |�  \�ܴX�$�h�. " ) ;  
 	 	 	 	 	 G e t M o d u l e F i l e N a m e W ( 0 ,   m o d u l e N a m e ,   M A X _ P A T H ) ;  
 	 	 	 	 	 s q l o g ( L "   -   tИ���  tǄ�:   % s " ,   w i . c l a s s N a m e ) ;  
 	 	 	 	 	 s q l o g ( L "   -   ���|�  ��\�" ) ;  
 	 	 	 	 	 s q l o g ( L "       % s " ,   m o d u l e N a m e ) ;  
 	 	 	 	 	 / /   s q l o g ( L "   -   C u r r e n t l y ,   t h e   m o d u l e   s u p p o r t s " ) ;  
 	 	 	 	 	 / /   s q l o g ( L "       w L a u n c h e r . e x e ,   C h a o s l a u n c h e r . e x e ,   S t a r c r a f t . e x e " ) ;  
 	 	 	 	 	 r e t u r n   F A L S E ;  
 	 	 	 	 }  
 	 	 	 	 / /   / *   g e t   m o d u l e   n a m e   * /  
 	 	 	 	 / /   G e t M o d u l e F i l e N a m e W ( 0 ,   m o d u l e N a m e ,   M A X _ P A T H ) ;  
 	 	 	 	 / /   i f   ( w c s s t r ( m o d u l e N a m e ,   L " S t a r c r a f t . e x e " )   | |   w c s s t r ( m o d u l e N a m e ,   L " s t a r c r a f t . e x e " ) )   {  
 	 	 	 	 / /   	 i s O n S t a r c r a f t   =   T R U E ;  
 	 	 	 	 / /   }   e l s e   i f   ( w c s s t r ( m o d u l e N a m e ,   L " w L a u n c h e r . e x e " )   | |    
 	 	 	 	 / /   	 w c s s t r ( m o d u l e N a m e ,   L " C h a o s l a u n c h e r . e x e " ) )   {  
 	 	 	 	 / /   	 s q l o g ( L " L a u n c h e r :   l o a d   c o m p l e t e " ) ;  
 	 	 	 	 / /   	 i s O n S t a r c r a f t   =   F A L S E ;  
 	 	 	 	 / /   }   e l s e   {  
 	 	 	 	 / /   	 s q l o g ( L " U n k n o w n   e x e c u t a b l e   a t t e m p t i n g   t o   l o a d   t h e   d l l " ) ;  
 	 	 	 	 / /   	 s q l o g ( L "   -   P a t h   o f   t h e   e x e c u t a b l e   i s   % s " ,   m o d u l e N a m e ) ;  
 	 	 	 	 / /   	 s q l o g ( L "   -   C u r r e n t l y ,   t h e   m o d u l e   s u p p o r t s " ) ;  
 	 	 	 	 / /   	 s q l o g ( L "       w L a u n c h e r . e x e ,   C h a o s l a u n c h e r . e x e ,   S t a r c r a f t . e x e " ) ;  
 	 	 	 	 / /   	 r e t u r n   F A L S E ;  
 	 	 	 	 / /   }  
 	 	 	 }  
 	 	 	 i f   ( c a l l e d C o u n t   = =   0   & &   i s O n S t a r c r a f t )   {  
 	 	 	 	 i f   ( h o o k _ i n i t ( h I n s t )   = =   F A L S E )   {  
 	 	 	 	 	 s q l o g ( L " ����lИ�ո���  \�ܴ  ��(�:   h o o k _ i n i t   h��  ���" ) ;  
 	 	 	 	 	 r e t u r n   F A L S E ;  
 	 	 	 	 }  
 	 	 	 	 i f   ( m e m c m p ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   c o d e _ 4 F 5 8 D 9 _ o l d ,   s i z e o f ( c o d e _ 4 F 5 8 D 9 _ n e w ) )   ! =   0 )   {  
 	 	 	 	 	 s q l o g ( L " ����lИ�ո���  \�ܴ  ��(�:   T����� �  t���h�D�  ���" ) ;  
 	 	 	 	 	 r e t u r n   F A L S E ;  
 	 	 	 	 }  
  
 	 	 	 	 / *  
 	 	 	 	   *   0 0 4 F 5 8 D 9   :   p u s h   0 x 0 0 0 0 0 0 D E  
 	 	 	 	   *   0 0 4 F 5 8 D E   :   p u s h   0 x 0 0 5 0 2 2 6 8   ( = =   " S t a r c r a f t \ S W A R \ l a n g \ g a m e d a t a . c p p " )  
 	 	 	 	   *   0 0 4 F 5 8 E 3   :   p u s h   e b x  
 	 	 	 	   *   0 0 4 F 5 8 E 4   :   p u s h   e b x  
 	 	 	 	   *   0 0 4 F 5 8 E 5   :   p u s h   e d i   ( c h e c k   f o r   f i l e N a m e   = =   " r e z \ \ q u i t 2 m n u . b i n " )  
 	 	 	 	   *   0 0 4 F 5 8 E 6   :   x o r   e c x ,   e c x  
 	 	 	 	   *   0 0 4 F 5 8 E 8   :   x o r   e a x ,   e a x  
 	 	 	 	   *   0 0 4 F 5 8 E A   :   c a l l   4 D 2 D 1 0   ( f i l e   r e a d i n g   f u n c t i o n )  
 	 	 	 	   * /  
 	 	 	 	 V i r t u a l P r o t e c t ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   1 0 ,   P A G E _ E X E C U T E _ R E A D W R I T E ,   & o l d P r o t e c t ) ;  
 	 	 	 	 m e m c p y ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   c o d e _ 4 F 5 8 D 9 _ n e w ,   1 0 ) ;  
 	 	 	 	 V i r t u a l P r o t e c t ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   1 0 ,   o l d P r o t e c t ,   & o l d P r o t e c t ) ;  
 	 	 	 	 s q l o g ( L " ����lИ�ո���  \�ܴ  1���" ) ;  
 	 	 	 }  
 	 	 	 b r e a k ;  
  
 	 	 c a s e   D L L _ T H R E A D _ D E T A C H :  
 	 	 	 i f   ( - - c a l l e d C o u n t   = =   0   & &   i s O n S t a r c r a f t )   {  
 	 	 	 	 V i r t u a l P r o t e c t ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   1 0 ,   P A G E _ E X E C U T E _ R E A D W R I T E ,   & o l d P r o t e c t ) ;  
 	 	 	 	 m e m c p y ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   c o d e _ 4 F 5 8 D 9 _ o l d ,   1 0 ) ;  
 	 	 	 	 V i r t u a l P r o t e c t ( ( v o i d   * ) 0 x 4 F 5 8 D 9 ,   1 0 ,   o l d P r o t e c t ,   & o l d P r o t e c t ) ;  
 	 	 	 	 s q l o g ( L " ����lИ�ո���  ��\�ܴ  1���" ) ;  
 	 	 	 }  
 	 	 	 b r e a k ;  
 	 }  
  
 	 r e t u r n   T R U E ;  
 }  
  
 B O O L   C A L L B A C K   E n u m W i n d o w s P r o c _ g e t C l a s s N a m e ( H W N D   h w n d ,   L P A R A M   l P a r a m ) {  
 	 s t r u c t   w i n d o w I n f o   * r e s   =   ( s t r u c t   w i n d o w I n f o   * ) l P a r a m ;  
 	 D W O R D   p r o c e s s ;  
 	 G e t W i n d o w T h r e a d P r o c e s s I d ( h w n d ,   & p r o c e s s ) ;  
 	 i f   ( G e t C u r r e n t P r o c e s s I d ( )   = =   p r o c e s s )   {  
 	 	 r e s - > h w n d   =   h w n d ;  
 	 	 G e t C l a s s N a m e W ( h w n d ,   r e s - > c l a s s N a m e ,   2 0 ) ;  
 	 	 r e t u r n   F A L S E ;  
 	 }  
 	 r e t u r n   T R U E ;  
 }  
 