# Vercel Deployment Guide

## Quick Deploy to Vercel

### Option 1: One-Click Deploy
[![Deploy with Vercel](https://vercel.com/button)](https://vercel.com/new/clone?repository-url=https://github.com/bniladridas/Llamaware/tree/main/web)

### Option 2: Manual Deploy

1. **Install Vercel CLI**
   ```bash
   npm i -g vercel
   ```

2. **Login to Vercel**
   ```bash
   vercel login
   ```

3. **Deploy from web directory**
   ```bash
   cd web
   vercel
   ```

4. **Follow the prompts:**
   - Set up and deploy? `Y`
   - Which scope? Choose your account
   - Link to existing project? `N`
   - Project name: `llamaware-agent`
   - Directory: `./` (current directory)

### Option 3: GitHub Integration

1. Push your code to GitHub
2. Go to [vercel.com](https://vercel.com)
3. Click "New Project"
4. Import your GitHub repository
5. Set root directory to `web`
6. Deploy!

## Environment Variables (Optional)

If you want to add analytics or other features:

```bash
# In Vercel dashboard, add these environment variables:
REACT_APP_GOOGLE_ANALYTICS_ID=your_ga_id
REACT_APP_MIXPANEL_TOKEN=your_mixpanel_token
```

## Custom Domain (Optional)

1. Go to your project in Vercel dashboard
2. Click "Settings" → "Domains"
3. Add your custom domain
4. Update DNS records as instructed

## Build Settings

Vercel will automatically detect:
- **Framework**: Create React App
- **Build Command**: `npm run build`
- **Output Directory**: `build`
- **Install Command**: `npm install`

## Performance Optimizations

The site includes:
- ✅ Static asset caching
- ✅ Gzip compression
- ✅ Image optimization
- ✅ Code splitting
- ✅ SEO meta tags
- ✅ PWA manifest

## Monitoring

After deployment, you can:
- View analytics in Vercel dashboard
- Monitor performance with Vercel Speed Insights
- Set up custom monitoring with your preferred service